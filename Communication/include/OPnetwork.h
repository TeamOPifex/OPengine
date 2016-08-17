#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "./Core/include/OPtypes.h"

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
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#endif

#define MAX_CLIENTS 16


enum OPnetworkOperation {
	OPNETWORK_OPERATION_ALLOW_DROPPED_PACKETS = 0,
	OPNETWORK_OPERATION_REQUEST_DROPPED_PACKETS = 1,
	OPNETWORK_OPERATION_SEQUENCED_PACKETS = 2,
};

enum OPnetworkType {
	OPNETWORK_CLIENT = 0,
	OPNETWORK_SERVER = 1
};

enum OPnetworkPrototcol {
	OPNETWORK_TCP = 0,
	OPNETWORK_UDP = 1
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

struct OPneworkData {
	fd_set master;
	fd_set read_fds;
	i32 fdmax;
	i32 clientIndex;
	struct sockaddr_storage peer_addr[MAX_CLIENTS];
	OPchar* peer_host[MAX_CLIENTS];
	OPchar* peer_port[MAX_CLIENTS];
};

struct OPneworkClientData {
	i32 fd;
};

struct OPnetwork {
	i32 ConnectSocket;
	OPnetworkType ConnectionType;
	OPnetworkPrototcol ConnectionProtocol;
	OPneworkData Data;
	void(*receive)(void*, i32, i32, OPchar*);
	void(*connected)(void*, i32);
	void(*disconnected)(void*, i32);
};

OPnetwork* OPnetworkCreate(OPnetworkType networkType, OPnetworkPrototcol protocol);
i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPchar* port);
i32 OPnetworkServerStart(OPnetwork* network, OPchar* port);
OPnetwork* OPnetworkAcceptClient(OPnetwork* network);
i32 OPnetworkReceive(OPnetwork* network, void* state);
i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size);
i32 OPnetworkShutdown(OPnetwork* network);
void OPnetworkDestroy(OPnetwork* network);
