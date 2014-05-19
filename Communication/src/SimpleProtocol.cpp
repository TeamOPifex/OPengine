#include "./include/SimpleProtocol.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"

OPnetworkProtocolSimple* OPnetworkProtocolSimpleCreate(OPnetworkType networkType) {

	OPnetworkProtocolSimple* protocol = (OPnetworkProtocolSimple*)OPalloc(sizeof(OPnetworkProtocolSimple));
	protocol->LastReceived = 0;
	protocol->LastSent = 0;
	protocol->Network = OPnetworkCreate(networkType);
	return protocol;
}

void Receive(void* state, i32 len, i8* data) {
	OPnetworkProtocolSimple* protocol = (OPnetworkProtocolSimple*)state;
	OPprotocolSimpleMessage message = *(OPprotocolSimpleMessage*)data;

	if (protocol->LastReceived < message.TimeStamp) {

		OPlog("packed received: %s", (data + sizeof(OPprotocolSimpleMessage)));
		message.Data = (i8*)OPalloc(message.DataLength);
		OPmemcpy(message.Data, (i8*)(data + sizeof(OPprotocolSimpleMessage)), message.DataLength);

		protocol->Receive(message);
		protocol->LastReceived = message.TimeStamp;
	}
}

i32 OPnetworkProtocolSimpleReceive(OPnetworkProtocolSimple* protocol, void(*receive)(OPprotocolSimpleMessage)) {
	protocol->Receive = receive;
	return OPnetworkReceive(protocol->Network, (void*)protocol, Receive);
}

i32 OPnetworkProtocolSimpleSend(OPnetworkProtocolSimple* protocol, OPtimer* timer, i8* data, i32 size) {
	i32 totalSize = sizeof(OPprotocolSimpleMessage) + size;
	i8* packedData = (i8*)OPalloc(totalSize * sizeof(i8));
	OPprotocolSimpleMessage* message = (OPprotocolSimpleMessage*)packedData;
	message->TimeStamp = ++protocol->LastSent;
	message->DataLength = size;
	message->Data = packedData + sizeof(OPprotocolSimpleMessage);
	memcpy(message->Data, data, size);

	OPlog("data: %s", data);
	OPlog("message data: %s", message->Data);
	OPlog("packed data: %s", (packedData + sizeof(OPprotocolSimpleMessage)));

	i32 result = OPnetworkSend(protocol->Network, packedData, totalSize);
	OPfree(message);
	return result;
}

void OPnetworkProtocolSimpleDestroy(OPnetworkProtocolSimple* protocol) {
	OPnetworkDestroy(protocol->Network);
	OPfree(protocol);
}