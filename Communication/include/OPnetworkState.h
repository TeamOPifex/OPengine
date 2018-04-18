#pragma once

#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkPacket.h"

class OPnetworkState {
public:
	virtual OPint Init(OPnetworkState*) = 0;
	virtual void Connected(OPnetworkSocket* socket) = 0;
	virtual void Disconnected(OPnetworkSocket* socket) = 0;
	virtual void Message(OPnetworkSocket* socket, OPnetworkPacket* packet) = 0;
	virtual OPint Exit(OPnetworkState*) = 0;
	void* Data;
	
	static OPint Destroy(OPnetworkState* state);
};