#include <net/Host.h>
#include <net/Address.h>
#include <net/Peer.h>

namespace eyos::net {

///////////////////////////////
/// Host  implementation   ///
//////////////////////////////

Host::Host(std::size_t peerCount,
    std::size_t channelLimit,
    std::uint32_t incomingBandwidth,
    std::uint32_t outgoingBandwidth)
{
    auto address { net::Address {} };
    enetHost = enet_host_create(&address.enetAddress, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    assertm(enetHost, "Failed creating host");
}
Host::Host(std::size_t peerCount,
    std::size_t channelLimit,
    std::uint32_t incomingBandwidth,
    std::uint32_t outgoingBandwidth,
    const net::Address& address)
{
    enetHost = enet_host_create(&address.enetAddress, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    assertm(enetHost, "Failed creating host");
}
Host::Host(std::size_t peerCount,
    std::size_t channelLimit,
    std::uint32_t incomingBandwidth,
    std::uint32_t outgoingBandwidth,
    net::Address&& address)
{
    enetHost = enet_host_create(&address.enetAddress, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    assertm(enetHost, "Failed creating host");
}

void Host::ChangeAddress(Address&& address) {
    std::size_t peerCount{ enetHost ->peerCount};
        std::size_t channelLimit{enetHost->channelLimit};
        std::uint32_t incomingBandwidth{enetHost->incomingBandwidth};
        std::uint32_t outgoingBandwidth{enetHost->outgoingBandwidth};
    enet_host_destroy(enetHost);
    enetHost = enet_host_create(&address.enetAddress, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    assertm(enetHost, "Failed creating host");
}
void Host::ChangeAddress(const Address& address) {
    std::size_t peerCount{ enetHost->peerCount };
    std::size_t channelLimit{ enetHost->channelLimit };
    std::uint32_t incomingBandwidth{ enetHost->incomingBandwidth };
    std::uint32_t outgoingBandwidth{ enetHost->outgoingBandwidth };
    enet_host_destroy(enetHost);
    enetHost = enet_host_create(&address.enetAddress, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    assertm(enetHost, "Failed creating host");
}
Peer Host::Connect(const net::Address& address, std::size_t channelCount, std::uint32_t data) const noexcept
{
    auto* peer_ptr { enet_host_connect(enetHost, &address.enetAddress, channelCount, data) };
    assertm(peer_ptr != nullptr, "No available peers for initiating an ENet connection.");
    return { peer_ptr };
}
[[nodiscard]] std::size_t Host::CountConnectedPeers() const noexcept
{
    return CountPeers(ENetPeerState::ENET_PEER_STATE_CONNECTED);
}
[[nodiscard]] std::size_t Host::CountPeers(ENetPeerState state) const noexcept
{
    auto count { 0 };
    auto peerCount{ static_cast<int>(enetHost->peerCount) };
    for (auto i { 0 }; i < peerCount; ++i) {
        if (enetHost->peers[i].state == state) {
            ++count;
        }
    }
    return count;
};

[[nodiscard]] auto Host::GetConnectedPeers() const noexcept
{
    std::vector<Peer> result {};
    auto peerCount{ static_cast<int>(enetHost->peerCount) };
    result.reserve(peerCount);
    for (auto i { 0 }; i < peerCount; ++i) {
        if (enetHost->peers[i].state == ENetPeerState::ENET_PEER_STATE_CONNECTED) {
            result.emplace_back<Peer>({ &enetHost->peers[i] });
        }
    }
    return result;
}
[[nodiscard]] Peer Host::GetPeer(std::size_t peerIdx)
{
    assertm(peerIdx < enetHost->peerCount, "Could not find peer in peer register");
    return { &enetHost->peers[peerIdx] };
}
bool Host::IsPeerValid(const Peer& peer)
{
    return peer.enetPeer->host == enetHost;
}

[[nodiscard]] std::size_t Host::PeerCount() const noexcept {
    return enetHost->peerCount;
}

Host::~Host()
{
    enet_host_destroy(enetHost);
}
}
