#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger, Maiko Steeman, Marjolein Kaal, Hannes Vernooij
Last updated on February 16th, 2020.

/see http://enet.bespin.org/Features.html
/note: concept of channel:
Since ENet will stall delivery of reliable packets to ensure proper sequencing, and consequently any packets of higher sequence number whether reliable or unreliable, in the event the reliable packet's predecessors have not yet arrived, this can introduce latency into the delivery of other packets which may not need to be as strictly ordered with respect to the packet that stalled their delivery.
To combat this latency and reduce the ordering restrictions on packets, ENet provides multiple channels of communication over a given connection. Each channel is independently sequenced, and so the delivery status of a packet in one channel will not stall the delivery of other packets in another channel.


*/
#include "NetFwd.h"

#include <vector>
namespace eyos::net {

class EYOS_API Host {
public:
    Host(std::size_t peerCount,
        std::size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth);

    Host(std::size_t peerCount,
        std::size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth,
        const Address& address);

    Host(std::size_t peerCount,
        std::size_t channelLimit,
        std::uint32_t incomingBandwidth,
        std::uint32_t outgoingBandwidth,
        Address&& address);

    Host(const Host&) = delete;
    Host(Host&& host) = default;
    Host& operator=(const Host& other) = delete;
    Host& operator=(Host&& other) = default;
    ~Host();

public:
    /*
    /note The peer returned will have not completed the connection until enet_host_service() notifies of an ENET_EVENT_TYPE_CONNECT event for the peer.
    /see http://enet.bespin.org/group__host.html#ga23b3ac206326b84f42fa91673f12fca9
    */
    Peer Connect(const Address& address, std::size_t channelCount, std::uint32_t data) const noexcept;
    [[nodiscard]] std::size_t CountConnectedPeers() const noexcept;
    [[nodiscard]] std::size_t PeerCount() const noexcept;
    [[nodiscard]] std::size_t CountPeers(ENetPeerState state) const noexcept;
    [[nodiscard]] auto GetConnectedPeers() const noexcept;
    [[nodiscard]]  Peer GetPeer(std::size_t peerIdx);
    void ChangeAddress(Address&& address);
    void ChangeAddress(const Address& address);
    bool IsPeerValid(const Peer& peer);

    ENetHost* enetHost { nullptr };
private:
    friend EYOS_API void Broadcast(const Host& peer, Packet&& packet, std::uint8_t channelID);
    friend class EYOS_API NetEventHandler;
};
}
