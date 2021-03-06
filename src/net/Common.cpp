#include <net/Common.h>
#include <net/Address.h>
#include <net/Host.h>
#include <net/Peer.h>
#include <net/Packet.h>
#include <net/streams/OutputStream.h>
#include <net/streams/InputStream.h>

namespace eyos::net {
    // Free function implementation
    [[nodiscard]] Address CreateAddress(std::uint16_t port, std::uint32_t host)
    {
        return { port, host };
    };
    [[nodiscard]] Address CreateAddress(std::uint16_t port, const std::string& hostName)
    {
        return { hostName, port };
    };
    [[nodiscard]] EYOS_API Host_ptr CreateHost(
        size_t peerCount,
        size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth)
    {
        return std::make_unique<Host>(peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    };
    [[nodiscard]] Host_ptr CreateHost(const Address& address,
        size_t peerCount,
        size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth)
    {
        return std::make_unique<Host>(peerCount, channelLimit, incomingBandwidth, outgoingBandwidth, address);
    };
    [[nodiscard]] Host_ptr CreateServer(
        std::uint16_t port,
        size_t peerCount,
        size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth)
    {
        auto address{ CreateAddress(port, "127.0.0.1") };
        return CreateHost(address, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    };

    [[nodiscard]] Host_ptr CreateClient(
        size_t peerCount,
        size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth)
    {
        return CreateHost(peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    };

    // consumes a packet
    bool SendPacket(const Peer& peer, Packet&& packet, std::uint8_t channelID)
    {
        return 0 == enet_peer_send(peer.enetPeer, channelID, packet.enetPacket);
    }

    void Broadcast(const Host& peer, Packet&& packet, std::uint8_t channelID)
    {
        enet_host_broadcast(peer.enetHost, channelID, packet.enetPacket);
    }

    EYOS_API OutputStream ToStream(InputStream&& stream)
    {
        return { stream.buffer,stream.capacity,stream.GetAllocator() };
    }
    EYOS_API InputStream ToStream(OutputStream&& stream)
    {
        return { stream.buffer,stream.capacity,stream.GetAllocator() };
    }
}