#include <net/Peer.h>
#include <net/Address.h>
#include <net/Utility.h>
namespace eyos::net {
///////////////////////////////
/// Peer  implementation   ///
//////////////////////////////

Peer::Peer(ENetPeer* peer)
    : enetPeer { peer }
{
    assertm(peer != nullptr, "Peer is not valid.\n");
}
Peer::Peer(Peer&& peer) noexcept
{
    this->enetPeer = peer.enetPeer;
}

[[nodiscard]] Address Peer::Address()
{
    return { enetPeer->address };
};

[[nodiscard]] std::uint32_t Peer::ConnectID() {
    return enetPeer->connectID;
}

bool Peer::IsConnected()
{
    return enetPeer->state == ENetPeerState::ENET_PEER_STATE_CONNECTED;
}

Peer& Peer::operator=(Peer&& peer) noexcept
{
    this->enetPeer = peer.enetPeer;
    return *this;
}
}
