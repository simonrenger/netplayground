#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger, Maiko Steeman, Marjolein Kaal, Hannes Vernooij
Last updated on February 16th, 2020.
*/
#include "NetFwd.h"
#include "Defines.h"
#include "Utility.h"

#include <string>
#include <cstring>

namespace eyos::net {

class EYOS_API Packet {
public:
    Packet(ENetPacket* packet, std::size_t typeSize = sizeof(unsigned int))
        : enetPacket{packet}, typeSize {
        typeSize
    }
    {
    }
    Packet(const Packet&) = delete;
    Packet(Packet&&) = default;
    Packet& operator=(const Packet&) = delete;
    Packet& operator=(Packet&&) = default;
    ~Packet() = default;
    /*
    * \remark: Should only be used in case it has not been send
    */
    void Destroy();
    template <typename PacketHeaderType>
    PacketHeaderType Type()
    {
        static_assert(std::is_enum_v<PacketHeaderType>, "Is not an Enum!");
        assertm(typeSize == sizeof(PacketHeaderType), "Saved size of type is different then actualy size!");
        PacketHeaderType type {};
        memcpy(&type, enetPacket->data, sizeof(PacketHeaderType));
        return type;
    }
    std::size_t Size() const {return enetPacket->dataLength;};
private:
    ENetPacket* enetPacket{ nullptr };
    std::size_t typeSize{0};
    // bit_cast => same byte code as reinterpret_cast and not UB
    std::byte* Data(std::size_t offset = 0) const { return net::bit_cast<std::byte*>(&enetPacket->data[offset]); };

    // friends:
    friend  bool EYOS_API SendPacket(const Peer& peer, Packet&& packet, std::uint8_t channelID = 0);
    friend void EYOS_API Broadcast(const Host& peer, Packet&& packet, std::uint8_t channelID);
    template <typename Data>
    friend  Packet&& AppendToPacket(Packet&& packet, Data&& data);
    template <typename Data>
    friend Packet&& AppendToPacket(Packet&& packet, Data&& data, std::size_t length);
    template <typename Data>
    friend  Packet&& AppendToPacket(Packet&& packet, Data& data);
    template <typename Data>
    friend  Packet&& AppendToPacket(Packet&& packet, const Data& data);
    template <typename Data>
    friend  Packet&& AppendToPacket(Packet&& packet, const Data& data, std::size_t length);
    template <typename PacketHeaderType>
    friend Packet StreamToPacket(const PacketHeaderType packetType, InputStream&& stream);
    friend  class EYOS_API InputStream;
    friend  class EYOS_API  OutputStream;
    friend  class EYOS_API  NetEventHandler;
};
}
