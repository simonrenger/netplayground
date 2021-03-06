#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger
Last updated on February 16th, 2020.
*/

#include "Defines.h"
#include "net/Packet.h"

#include <cstddef>
#include <cstdint>
#include <string>
namespace eyos::net {
// forward declareations:
class OutputStream;
class Allocator;
EYOS_API Allocator& GetNetworkAllocator();

class EYOS_API InputStream{
    public:
    InputStream();
    ~InputStream();
    InputStream(std::size_t bufferSize);
    InputStream(Allocator& allocator);
    InputStream(Allocator& allocator,std::size_t bufferSize);
    InputStream(const InputStream&) = delete;
    InputStream(InputStream&&) = default;
    InputStream& operator=(const InputStream&) = delete;
    InputStream& operator=(InputStream&&) = delete;
    public:
    [[nodiscard]]std::size_t GetRemainingSize();
    [[nodiscard]] Allocator& GetAllocator();
    InputStream& operator<<(const bool& value);
    InputStream& operator<<(const char& value);
    InputStream& operator<<(const std::string& value);
    InputStream& operator <<(const char* value);
    InputStream& operator<<(const std::int8_t& value);
    InputStream& operator<<(const std::uint8_t& value);
    InputStream& operator<<(const std::int16_t& value);
    InputStream& operator<<(const std::uint16_t& value);
    InputStream& operator<<(const std::int32_t& value);
    InputStream& operator<<(const std::uint32_t& value);
    InputStream& operator<<(const std::int64_t& value);
    InputStream& operator<<(const std::uint64_t& value);
    private:
    InputStream(std::byte* buffer, std::size_t size, Allocator& allocator = GetNetworkAllocator());
    private:
    Allocator& allocator;
    std::size_t head{};
    std::size_t capacity{};
    std::byte* buffer{};
    template <typename PacketHeaderType>
    friend Packet StreamToPacket(const PacketHeaderType packetType, InputStream&& stream);
    friend EYOS_API OutputStream ToStream(InputStream&& stream);
    friend EYOS_API InputStream ToStream(OutputStream&& stream);
};

template <typename PacketHeaderType>
[[nodiscard]]  Packet StreamToPacket(const PacketHeaderType packetType,InputStream&& stream) {
    auto packet{ CreateEmptyPacket(packetType) };
    auto privSize{ packet.enetPacket->dataLength };
    auto newSize{ privSize + stream.capacity };
    enet_packet_resize(packet.enetPacket, newSize);
    memcpy(&packet.enetPacket->data[privSize], stream.buffer, stream.capacity);
    return packet;
}

}