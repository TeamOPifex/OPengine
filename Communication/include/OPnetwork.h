#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "./Core/include/OPtypes.h"

#include "./Communication/include/Enums/OPnetworkType.h"
#include "./Communication/include/Enums/OPnetworkProtocol.h"
#include "./Communication/include/Enums/OPnetworkOperation.h"
#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkData.h"

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512




struct OPnetwork {
	i32 connectedSocket;
	OPnetworkType::Enum networkType;
	OPnetworkProtocol::Enum networkProtocol;
	OPneworkData networkData;

	void(*receive)(void*, i32, i32, OPchar*);
	void(*connected)(void*, i32);
	void(*disconnected)(void*, i32);

	i32 ClientConnect(const OPchar* address, const OPchar* port);
	i32 ServerStart(const OPchar* port);
	OPnetwork* AcceptClient();
	
	i32 Receive(void* state);
	i32 Send(i8* data, i32 size);

	i32 Shutdown();
	void Destroy();

	static OPnetwork* Create(OPnetworkType::Enum networkType, OPnetworkProtocol::Enum protocol);
	static i32 Startup();
	static ui64 LookupAddress(const OPchar *kpAddress, OPchar** resolved);
	static void LogError(const OPchar* message);
	static i32 GetAddress(const OPchar* address, const OPchar* port, OPnetworkAddress* networkAddress, OPnetworkProtocol::Enum protocol);

	private:
		i32 ServerStartUDP(OPnetwork* network);
		i32 ServerStartTCP(OPnetwork* network);
		i32 ReceiveServer(void* state);
		i32 ReceiveClient(void* state);
};
