#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"

struct OPnetworkServer {
	OPnetwork network;
	OPnetworkSocket serverSocket;
	OPnetworkSocket clients[MAX_CLIENTS];
	ui32 clientIndex = 0;

	void Init(OPnetworkSocketType::Enum socketType, const OPchar* port);
	void Update();
	bool Send(void* data, ui32 size);
	void Destroy();
    void UpdateTCP();
    void UpdateUDP();
	int HandleNewConnection();
};