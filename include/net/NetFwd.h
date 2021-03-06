#pragma once
/*
Eyos Source Code License v1.0
Copyright (c) 2019-2020 Simon Renger, Maiko Steeman, Marjolein Kaal, Hannes Vernooij
Last updated on February 16th, 2020.
*/
#include <Defines.h>
#include <enet/enet.h>

#include <cstdint>
#include <memory>
namespace eyos::net {

	class EYOS_API Allocator;

	EYOS_API Allocator& GetNetworkAllocator();
	EYOS_API void InitializeNetwork();
	EYOS_API void DeinitializeNetwork();

	// Network Lib Wrappers forward:
	class EYOS_API Address;
	class EYOS_API Host;
	class EYOS_API Peer;
	class EYOS_API Packet;
	class EYOS_API InputStream;
	class EYOS_API OutputStream;



	using Event = ENetEvent;

	//Pointers:
	using Host_ptr = std::unique_ptr<Host>;
	// Network Action
	class EYOS_API NetEventHandler;
};