#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkSelect.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"

typedef void(*OPnetworkServerReceiveCallback)(OPnetworkSocket*, void*, ui32);

#define MAX_LINE 80

struct OPnetworkServer {	
	OPnetworkSocket serverSocket4;
	OPnetworkSocket serverSocket6;
	OPnetworkSelect selector;
	OPnetworkProtocolType::Enum protocolType;

	OPnetworkSocket clients[MAX_CLIENTS];
	ui32 clientIndex = 0;

	char buf[MAX_LINE];

	void Init(OPnetworkProtocolType::Enum protocol, ui32 port);
	void Update(ui64 elapsed);
	bool Send(OPnetworkPacket* packet);
	void Destroy();
private: 
	void HandleServerReceive(OPnetworkSocket* serverSocket);
};