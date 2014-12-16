#ifndef OPENGINE_COMMUNICATION_SIMPLEPROTOCOL
#define OPENGINE_COMMUNICATION_SIMPLEPROTOCOL

#include "./Communication/include/OPnetwork.h"
#include "./Core/include/OPtimer.h"

typedef struct {
	i32 TimeStamp;
	i32 DataLength;
	i8* Data;
} OPprotocolSimpleMessage;

typedef struct {
	OPnetwork* Network;
	i32 LastReceived;
	i32 LastSent;
	void(*Receive)(OPprotocolSimpleMessage);
} OPnetworkProtocolSimple;

OPnetworkProtocolSimple* OPnetworkProtocolSimpleCreate(OPnetworkType networkType);
i32 OPnetworkProtocolSimpleReceive(OPnetworkProtocolSimple* protocol, void(*receive)(OPprotocolSimpleMessage));
i32 OPnetworkProtocolSimpleSend(OPnetworkProtocolSimple* protocol, OPtimer* timer, i8* data, i32 size);
void OPnetworkProtocolSimpleDestroy(OPnetworkProtocolSimple* protocol);

#endif