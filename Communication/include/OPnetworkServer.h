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

	OPnetworkServerReceiveCallback receiveCallback = NULL;
	OPnetworkClientConnectedCallback clientConnectedCallback = NULL;
	OPnetworkClientDisconnectedCallback clientDisconnectedCallback = NULL;

	void Init(OPnetworkProtocolType::Enum protocolType, ui32 port);
	void Update();
	bool Send(void* data, ui32 size);
	void Destroy();
    void UpdateTCP();
    void UpdateUDP();
	int HandleNewConnection();
	int Select();

	inline void SetReceiveCallback(OPnetworkServerReceiveCallback cb) {
		receiveCallback = cb;
	}

	inline void SetClientConnectedCallback(OPnetworkClientConnectedCallback cb) {
		clientConnectedCallback = cb;
	}

	inline void SetClientDisconnectedCallback(OPnetworkClientDisconnectedCallback cb) {
		clientDisconnectedCallback = cb;
	}

};