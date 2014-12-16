#ifndef OPENGINE_COMMUNICATION_NETWORK
#define OPENGINE_COMMUNICATION_NETWORK

#include "./Core/include/Types.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512



enum OPnetworkOperation {
	OPNETWORK_OPERATION_ALLOW_DROPPED_PACKETS = 0,
	OPNETWORK_OPERATION_REQUEST_DROPPED_PACKETS = 1,
	OPNETWORK_OPERATION_SEQUENCED_PACKETS = 2,
};

enum OPnetworkType {
	OPNETWORK_CLIENT = 0,
	OPNETWORK_SERVER = 1
};

typedef struct {
	OPchar* Buffer;
	i32 Length;
} OPnetworkPacketRaw;

typedef struct {
	OPnetworkPacketRaw* RawPacket;
	// Header
	i32 ClientId;
	// Message
	OPchar* Message;
} OPnetworkPacket;

typedef struct {
	ui32 ConnectSocket;
	OPnetworkType ConnectionType;
} OPnetwork;

OPnetwork* OPnetworkCreate(OPnetworkType networkType);
i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, i32 port);
i32 OPnetworkServerStart(OPnetwork* network, i32 port);
OPnetwork* OPnetworkAcceptClient(OPnetwork* network);
i32 OPnetworkReceive(OPnetwork* network, void* state, void(*receive)(void*, i32, OPchar*));
i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size);
i32 OPnetworkShutdown(OPnetwork* network);
void OPnetworkDestroy(OPnetwork* network);

#endif