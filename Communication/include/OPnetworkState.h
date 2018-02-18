#pragma once

#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkPacket.h"

class OPnetworkState {
public:
	virtual OPint Init(OPnetworkState*) = 0;
	virtual void ClientConnected(OPnetworkSocket* socket) = 0;
	virtual void ClientDisconnected(OPnetworkSocket* socket) = 0;
	virtual void OnMessage(OPnetworkSocket* socket, OPnetworkPacket* packet) = 0;
	virtual OPint Exit(OPnetworkState*) = 0;
	void* Data;
	
	static void Change(OPnetworkState* targetState);
	static OPint Destroy(OPnetworkState* state);
};

extern OPnetworkState* ActiveNetworkState;