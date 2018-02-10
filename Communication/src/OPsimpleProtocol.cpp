#include "./Communication/include/OPsimpleProtocol.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

void Connected(OPnetworkProtocolSimple* state, i32 index);
void Disconnected(OPnetworkProtocolSimple* state, i32 index);
void Receive(OPnetworkProtocolSimple* state, i32 len, i32 index, i8* data);

OPnetworkProtocolSimple* OPnetworkProtocolSimpleCreate(OPnetworkType::Enum networkType) {
	OPnetworkProtocolSimple* protocol = (OPnetworkProtocolSimple*)OPalloc(sizeof(OPnetworkProtocolSimple));
	// protocol->Network = OPnetwork::Create(networkType, OPnetworkProtocol::OPNETWORK_UDP);
	// protocol->Network->receive = (void(*)(void*, i32, i32, OPchar*))Receive;
	// protocol->Network->connected = (void(*)(void*, i32))Connected;
	// protocol->Network->disconnected = (void(*)(void*, i32))Disconnected;
	return protocol;
}

void Connected(OPnetworkProtocolSimple* state, i32 index) {
	OPlogInfo("Network Protocol Simple Connected: %d", index);
}

void Disconnected(OPnetworkProtocolSimple* state, i32 index) {
	OPlogInfo("Network Protocol Simple Disconnected: %d", index);
}

void Receive(OPnetworkProtocolSimple* state, i32 index, i32 len, i8* data) {
	// OPprotocolSimpleMessage message = *(OPprotocolSimpleMessage*)data;

	// if (state->Network->networkType == OPnetworkType::OPNETWORK_CLIENT || state->LastReceived[index] < message.TimeStamp) {

	// 	OPlog("packed received: %s", (data + sizeof(OPprotocolSimpleMessage)));
	// 	message.Data = (i8*)OPalloc(message.DataLength);
	// 	OPmemcpy(message.Data, (i8*)(data + sizeof(OPprotocolSimpleMessage)), message.DataLength);

	// 	state->Receive(message);
	// 	if (state->Network->networkType == OPnetworkType::OPNETWORK_SERVER) {
	// 		state->LastReceived[index] = message.TimeStamp;
	// 	}
	// }
	// else {
	// 	OPlog("Message Received Late: %d last was %d", message.TimeStamp, state->LastReceived[index]);
	// }
}

i32 OPnetworkProtocolSimpleReceive(OPnetworkProtocolSimple* protocol, void(*receive)(OPprotocolSimpleMessage)) {
	//protocol->Receive = receive;
	//return protocol->Network->Receive((void*)protocol);
	return 0;
}

i32 OPnetworkProtocolSimpleSend(OPnetworkProtocolSimple* protocol, OPtimer* timer, i8* data, i32 size) {
	// i32 totalSize = sizeof(OPprotocolSimpleMessage) + size;
	// i8* packedData = (i8*)OPalloc(totalSize * sizeof(i8));
	// OPprotocolSimpleMessage* message = (OPprotocolSimpleMessage*)packedData;
	// message->TimeStamp = ++protocol->LastSent;
	// message->DataLength = size;
	// message->Data = packedData + sizeof(OPprotocolSimpleMessage);
	// memcpy(message->Data, data, size);

	// OPlog("data: %d", *data);
	// // OPlog("message data: %s", message->Data);
	// // OPlog("packed data: %s", (packedData + sizeof(OPprotocolSimpleMessage)));

	// i32 result = protocol->Network->Send(packedData, totalSize);
	// OPlog("Send Result %d", result);
	// OPfree(message);
	// return result;
	return 0;
}

void OPnetworkProtocolSimpleDestroy(OPnetworkProtocolSimple* protocol) {
	// protocol->Network->Destroy();
	// OPfree(protocol);
}
