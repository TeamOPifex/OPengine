#ifndef OPENGINE_COMMUNICATION_WEBSERVER
#define OPENGINE_COMMUNICATION_WEBSERVER

#include "./Core/include/OPtypes.h"
#include "./External/Mongoose/include/Mongoose.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"

#define MAX_MESSAGES_PER_FRAME 10

struct OPwebServer {
	struct mg_server* Server;
	// WebSocket Messages
	// Key, Data, Data Length
	void(*Receive)(i8*, i8*, i32);
	OPhashMap* WebSocketKeys;

	ui32 messageCount;
	OPstream* messages[MAX_MESSAGES_PER_FRAME];
	//OPlist* WebSocketMessages;
};

struct OPWebServerHandlerContainer {
	void(*handler)(OPstream*, void*);
	void* param;
};

//struct OPWebServerMessagesContainer {
//	OPuint messageCount;
//	OPstream** messages;
//};

OPwebServer* OPwebServerCreate(OPchar* port);
void OPwebServerUpdate(OPwebServer* server);
void OPwebServerOnKey(OPwebServer* server, OPchar* key, void(*handler)(OPstream*, void*), void* param);
void OPwebServerQueue(OPwebServer* server, OPchar* key, i8* data, ui32 datalen);
void OPwebServerDestroy(OPwebServer* server);

inline void OPwebServerQueue(OPwebServer* server, OPchar* key, OPchar* str) {
	i8 buff[1024];
	ui32* casted = (ui32*)buff;
	casted[0] = strlen(str);
	OPmemcpy(&casted[1], str, casted[0] * sizeof(OPchar));
	OPwebServerQueue(server, key, buff, casted[0] + sizeof(ui32));
}

#endif
