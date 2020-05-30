#include <algorithm>

#include <net/Host.h>
#include <net/Peer.h>
#include <net/Packet.h>
#include <net/NetEventHandler.h>

#include <eventpp/eventqueue.h>

namespace eyos::net {
    //////////////////////////////////////
    /// NetEventHandler implementation ///
    /////////////////////////////////////
    void NetEventHandler::SendAndReceive(const eyos::net::Host_ptr& host, std::size_t timeout) {
        SendAndReceive(*host, timeout);
    }
    void NetEventHandler::SendAndReceive(const eyos::net::Host& host, std::size_t timeout) {
        // iterate over all outgoing packets
        ProcessSendQ();
        //recive ingoing packets and send outgoing packets
        net::Event event{};
        while (enet_host_service(host.enetHost, &event, timeout) > 0) {
            auto eventType{ static_cast<NetEvents>(event.type) };
            eventsTemp.emplace_back(eventType,event);
        }
        {
            std::lock_guard<std::mutex> queueListLock(eventsMtx);
            events.insert(
                events.end(),
                std::make_move_iterator(eventsTemp.begin()),
                std::make_move_iterator(eventsTemp.end())
            );
        }
        eventsTemp.clear();
    }
    void NetEventHandler::Send(eyos::net::Peer peer, eyos::net::Packet&& packet,std::uint8_t channelID) {
            std::lock_guard<std::mutex> queueListLock(packetsMtx);
            packetsToPeer.emplace_back(std::move(peer),std::move(packet), channelID);
    }
    void NetEventHandler::Broadcast(eyos::net::Host_ptr& host, eyos::net::Packet&& packet, std::uint8_t channleID) {
        std::lock_guard<std::mutex> queueListLock(packetsToHostMtx);
        packetsToHost.emplace_back(host.get(), std::move(packet), channleID);
    }
    void NetEventHandler::ProcessSendQ() {

        {
            std::lock_guard<std::mutex> queueListLock(packetsMtxTemp);
            packetsTemp.insert(
                packetsTemp.end(),
                std::make_move_iterator(packetsToPeer.begin()),
                std::make_move_iterator(packetsToPeer.end())
            );
        }

        for (auto&& [peer, packet, channelID] : packetsTemp) {
            net::SendPacket(peer, std::move(packet), channelID);
        }
        {
            std::lock_guard<std::mutex> queueListLock(packetsMtxTemp);
            packetsTemp.clear();
        }

        {
            std::lock_guard<std::mutex> queueListLock(packetsToHostMtxTemp);
            packetsToHostTemp.insert(
                packetsToHostTemp.end(),
                std::make_move_iterator(packetsToHost.begin()),
                std::make_move_iterator(packetsToHost.end())
            );
        }

        for (auto& [host, packet, channelID] : packetsToHostTemp) {
            net::Broadcast(*host, std::move(packet), channelID);
        }

    }
    void NetEventHandler::Process() {
            {
                std::lock_guard<std::mutex> queueListLock(eventsMtx);
                eventsProcessTemp.insert(
                    eventsProcessTemp.end(),
                    std::make_move_iterator(events.begin()),
                    std::make_move_iterator(events.end())
                );
                events.clear();
            }
            // process them:
            for (const auto& [event,eventData] : eventsProcessTemp) {
                switch (event)
                {
                case NetEvents::Connect:
                    printf("A new client connected from %x:%u.\n",
                        eventData.peer->address.host,
                        eventData.peer->address.port);
                    /* Store any relevant client information here. */
                    eventData.peer->data = "Client information";
                    break;
                case NetEvents::Receive:
                    printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                        eventData.packet->dataLength,
                        eventData.packet->data,
                        eventData.peer->data,
                        eventData.channelID);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(eventData.packet);

                    break;

                case NetEvents::Disconnect:
                    printf("%s disconnected.\n", eventData.peer->data);
                    /* Reset the peer's client information. */
                    eventData.peer->data = NULL;
                    break;
                }
            }
            eventsProcessTemp.clear();
    }
}
