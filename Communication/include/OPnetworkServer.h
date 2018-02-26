#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkSelect.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"

typedef void(*OPnetworkServerReceiveCallback)(OPnetworkSocket*, void*, ui32);

#define MAX_LINE 80

struct OPnetworkServer {	
	OPnetworkSocket serverSocket;
	OPnetworkSelect selector;

	OPnetworkSocket clients[MAX_CLIENTS];
	ui32 clientIndex = 0;

	char buf[MAX_LINE];

	void Init(OPnetworkProtocolType::Enum protocolType, ui32 port);
	void Update();
	bool Send(OPnetworkPacket* packet);
	void Destroy();
};