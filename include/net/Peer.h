#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger, Maiko Steeman, Marjolein Kaal, Hannes Vernooij
Last updated on February 16th, 2020.
*/
#include "NetFwd.h"

#include <optional>
namespace eyos::net {
/*
    * \important contains an NONE owning pointer
    */
class EYOS_API Peer {
public:
    Peer(ENetPeer*);
    ~Peer() = default;
    Peer(const Peer&) = delete;
    Peer(Peer&& peer) noexcept;
    Peer& operator=(const Peer&) = delete;
    Peer& operator=(Peer&& peer) noexcept;

public:
    [[nodiscard]] net::Address Address();
    [[nodiscard]] std::uint32_t ConnectID();
    bool IsConnected();


    ENetPeer* enetPeer { nullptr };
private:
    //friends:
    friend EYOS_API bool SendPacket(const Peer& peer, Packet&& packet);
    friend EYOS_API void Broadcast(const Host& peer, Packet&& packet, std::uint8_t channelID);
    friend class EYOS_API Host;
};
}
