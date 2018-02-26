#pragma once

#include "./Communication/include/OPnetworkState.h"

class OPserverProtocol : public OPnetworkState {	
	OPint Init(OPnetworkState* prev);
	void Connected(OPnetworkSocket* socket);
	void Disconnected(OPnetworkSocket* socket);
	void Message(OPnetworkSocket* socket, OPnetworkPacket* packet);
	OPint Exit(OPnetworkState* prev);
};

class OPclientProtocol : public OPnetworkState {	
	OPint Init(OPnetworkState* prev);
	void Connected(OPnetworkSocket* socket);
	void Disconnected(OPnetworkSocket* socket);
	void Message(OPnetworkSocket* socket, OPnetworkPacket* packet);
	OPint Exit(OPnetworkState* prev);
};