#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"

struct OPnetworkClient {
	OPnetwork network;
    OPnetworkSocket socket;

	void Init(OPnetworkSocketType::Enum socketType, const OPchar* address, const OPchar* port);
	void Update();
	bool Send(void* data, ui32 size);
    void Destroy();
};
