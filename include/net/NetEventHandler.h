#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger, Maiko Steeman, Marjolein Kaal, Hannes Vernooij
Last updated on February 16th, 2020.
*/
#include "NetFwd.h"
#include <functional>
#include <mutex>

namespace eyos::net {
    enum  class NetEvents {
        None,
        Connect,
        Disconnect,
        Receive
    };

    // Network Handler:
    class EYOS_API NetEventHandler {
    public:
        NetEventHandler() = default;
        ~NetEventHandler() = default;
    public:
        /*
        /note calles enet_host_service underneath which is responsible for receive and sending
        */
        void SendAndReceive(const eyos::net::Host& host,std::size_t timeout = 1000);
        void SendAndReceive(const eyos::net::Host_ptr& host,std::size_t timeout = 1000);
        void Process();
        // consumes a packet
        void Send(eyos::net::Peer peer, eyos::net::Packet&& packet, std::uint8_t channleID = 0);
        void Broadcast(eyos::net::Host_ptr& host, eyos::net::Packet&& packet, std::uint8_t channleID = 0);
    private:
        void ProcessSendQ();
    private:
        std::vector<std::pair<NetEvents, net::Event>> events{};
        std::vector<std::pair<NetEvents, net::Event>> eventsTemp{};
        std::vector<std::pair<NetEvents, net::Event>> eventsProcessTemp{};
        std::vector<std::tuple<net::Peer, net::Packet,std::uint8_t>> packetsToPeer{};
        std::vector<std::tuple<net::Peer, net::Packet, std::uint8_t>> packetsTemp{};
        std::vector<std::tuple<net::Host*, net::Packet, std::uint8_t>> packetsToHost{};
        std::vector<std::tuple<net::Host*, net::Packet, std::uint8_t>> packetsToHostTemp{};
        std::mutex eventsMtx{};
        std::mutex eventsMtxTemp{};
        std::mutex packetsMtx{};
        std::mutex packetsMtxTemp{};
        std::mutex packetsToHostMtx{};
        std::mutex packetsToHostMtxTemp{};
    };
}