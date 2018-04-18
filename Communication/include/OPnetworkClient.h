#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkSelect.h"
#include "./Communication/include/OPnetworkState.h"

typedef void(*OPnetworkClientReceiveCallback)(void*, ui32);

#define MAX_LINE 80

struct OPnetworkClient {
	OPnetworkSocket clientSocket;
	OPnetworkSelect selector;
	char buf[MAX_LINE];

	OPnetworkState* ActiveNetworkState = NULL;
	bool verified = false;

#ifdef _DEBUG
	ui32 simulatedLag = 0;
	ui32 simulatedJitter = 0;
	f32 simulatedPacketLossPercent = 0;
#endif

	void Init(OPnetworkProtocolType::Enum protocolType, const OPchar* address, ui32 port);
	void Update();
	bool Send(OPnetworkPacket* packet);
    void Destroy();
};
