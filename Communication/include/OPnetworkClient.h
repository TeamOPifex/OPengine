#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkSelect.h"

typedef void(*OPnetworkClientReceiveCallback)(void*, ui32);

#define MAX_LINE 80

struct OPnetworkClient {
	OPnetwork network;
	OPnetworkSocket clientSocket;
	OPnetworkSelect selector;


	struct sockaddr_in sin, cin;
	socklen_t addr_len;

	int sockfd, maxfdp;
	char *msg = "client";
	char buf[MAX_LINE];


	OPnetworkClientReceiveCallback receiveCallback = NULL;

	void Init(OPnetworkProtocolType::Enum protocolType, const OPchar* address, ui32 port);
	void Update();
	bool Send(void* data, ui32 size);
    void Destroy();
	i32 Select(fd_set* read, fd_set* write, fd_set* except);

	inline void SetReceiveCallback(OPnetworkClientReceiveCallback cb) {
		receiveCallback = cb;
	}
};
