#pragma once
#ifndef OPEngine_Communication_UDP
#define OPEngine_Communication_UDP

#include "./../../Core/include/Core.h"
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <errno.h>

#define h_addr h_addr_list[0]
typedef struct sockaddr_in SockAddr;
typedef struct sockaddr sockaddr;
typedef struct hostent HostEnt;

typedef struct{
	OPint _socket;
	SockAddr _sin;
} OPconnection;


OPconnection* OPhost(OPint port, OPprotocol protocol, int maxIncomming);
OPconnection* OPconnect(OPchar* address, OPint port, OPprotocol protocol);

OPint OPreceive(OPconnection* con, void* data, OPuint size);
OPint OPsend(OPconnection* con, void* data, OPuint size);

OPuint OPdisconnect(OPconnection* connection);
#endif


typedef enum{
	UDP,
	TCP
} OPprotocol;

#endif
