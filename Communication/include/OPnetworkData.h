#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/OPnetworkPlatform.h"

#define MAX_CLIENTS 16

struct OPneworkData {
	fd_set master;
	fd_set read_fds;
	i32 fdmax;
	i32 clientIndex;
	struct sockaddr_storage peer_addr[MAX_CLIENTS];
	OPchar* peer_host[MAX_CLIENTS];
	OPchar* peer_port[MAX_CLIENTS];
};