#include <net/Net.h>
#include <net/streams/InputStream.h>
#include <net/streams/OutputStream.h>

#include <eventpp/eventqueue.h>

#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
#include <string>

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


using namespace eyos;
std::atomic<bool> run{ true };

struct Client {
    const std::uint32_t connectId{};
};

enum class Commands {
    Welcome,
    Turn
};

struct PacketSystem{
    net::Host& host;
    std::queue<net::Packet> packets{};
    void OnReceive(net::Packet&& packet, [[maybe_unused]] std::uint32_t eventData) {
        if (packet.Type<Commands>() == Commands::Welcome) {
            std::string name{};
            net::OutputStream out{ std::move(packet) };
            out >> name;
            std::cout << name << '\n';
        }
    };
};

struct PeerSystem {
    net::Host& host;
    std::vector<Client>& clients;
    void OnConnect(net::Peer&& who, [[maybe_unused]] std::uint32_t eventData) {
        Client client{ who.ConnectID()};
        clients.emplace_back(std::move(client));
        std::cout << "Connected\n";
    };
    void OnDisconnect(net::Peer&& who, [[maybe_unused]] std::uint32_t eventData) {
        auto predict{ [connectId = who.ConnectID()] (const auto& client) {
            return client.connectId == connectId;
        } };
        auto itr{ std::find_if(clients.begin(), clients.end(),predict) };
        if (itr != clients.end() && clients.size() != 1) {
            std::swap(itr, clients.end()--);
        }
        clients.pop_back();
    };
};



int main()
{
    char serverOrClient{};
    char send{};
    std::cout << "##### EyosNet #####\n";
    std::cout << "Server/Client? [s|c]: ";
    std::cin >> serverOrClient;
    std::cout << '\n';

    net::InitializeNetwork();
    net::Host_ptr host{ nullptr };
    if (serverOrClient == 'c') {
        host = std::move(net::CreateClient());
        auto address{ net::CreateAddress(1234,"127.0.0.1") };
        auto peer = host->Connect(address, 2, 42);
        std::cout << "Run in Client Mode\n";
        net::Event event{};
    }
    else {
        host = std::move(net::CreateServer(1234));
        std::cout << "Run in Server Mode. Listen to port 1234\n";
    }
        auto eventHandler{ net::NetEventHandler{} };
        std::thread thread([&host,&eventHandler]() {
            while (run) {
                eventHandler.SendAndReceive(host);
            }

        });
        std::thread processThread([&eventHandler]() {
            while (run) {
                eventHandler.Process();
            }
        });
        while (run) {
            std::cout << "Do you want to send a message? [y|n]\n";
            std::cin >> send;
            if (send == 'y') {
                net::InputStream in{};
                in << "Hallo World";
                auto packet{ net::StreamToPacket(Commands::Turn, std::move(in)) };
                eventHandler.Send(host->GetPeer(0), std::move(packet));
            }
            clear();
        }
    processThread.join();
    thread.join();
    net::DeinitializeNetwork();
}