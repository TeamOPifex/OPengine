#ifndef OPENGINE_COMMUNICATION_NETWORK
#define OPENGINE_COMMUNICATION_NETWORK

#include "./Core/include/OPtypes.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef OPIFEX_UNIX

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#endif


enum OPnetworkOperation {
	OPNETWORK_OPERATION_ALLOW_DROPPED_PACKETS = 0,
	OPNETWORK_OPERATION_REQUEST_DROPPED_PACKETS = 1,
	OPNETWORK_OPERATION_SEQUENCED_PACKETS = 2,
};

enum OPnetworkType {
	OPNETWORK_CLIENT = 0,
	OPNETWORK_SERVER = 1
};

struct OPnetworkPacketRaw {
	OPchar* Buffer;
	i32 Length;
};

struct OPnetworkPacket {
	OPnetworkPacketRaw* RawPacket;
	// Header
	i32 ClientId;
	// Message
	OPchar* Message;
};

struct OPneworkServerData {
	fd_set master;
	fd_set read_fds;
	i32 fdmax;
};

struct OPneworkClientData {
	i32 fd;
};

struct OPnetwork {
	i32 ConnectSocket;
	OPnetworkType ConnectionType;
	void* Data;
};

OPnetwork* OPnetworkCreate(OPnetworkType networkType);
i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPchar* port);
i32 OPnetworkServerStart(OPnetwork* network, OPchar* port);
OPnetwork* OPnetworkAcceptClient(OPnetwork* network);
i32 OPnetworkReceive(OPnetwork* network, void* state, void(*receive)(void*, i32, OPchar*));
i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size);
i32 OPnetworkShutdown(OPnetwork* network);
void OPnetworkDestroy(OPnetwork* network);

#endif
