#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Core/include/OPtimer.h"

struct OPprotocolSimpleMessage {
	i32 TimeStamp;
	i32 DataLength;
	i8* Data;
};

struct OPnetworkProtocolSimple {
	OPnetwork* Network;
	i32 LastReceived[MAX_CLIENTS];
	i32 LastSent;
	void(*Receive)(OPprotocolSimpleMessage);
};

OPnetworkProtocolSimple* OPnetworkProtocolSimpleCreate(OPnetworkType networkType);
i32 OPnetworkProtocolSimpleReceive(OPnetworkProtocolSimple* protocol, void(*receive)(OPprotocolSimpleMessage));
i32 OPnetworkProtocolSimpleSend(OPnetworkProtocolSimple* protocol, OPtimer* timer, i8* data, i32 size);
void OPnetworkProtocolSimpleDestroy(OPnetworkProtocolSimple* protocol);