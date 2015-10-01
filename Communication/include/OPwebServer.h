#ifndef OPENGINE_COMMUNICATION_WEBSERVER
#define OPENGINE_COMMUNICATION_WEBSERVER

#include "./Core/include/OPtypes.h"
#include "./External/Mongoose/include/Mongoose.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"

struct OPWebServer {
	struct mg_server* Server;
	// WebSocket Messages
	// Key, Data, Data Length
	void(*Receive)(i8*, i8*, i32);
	OPhashMap* WebSocketKeys;
	OPlist* WebSocketMessages;
};

struct OPWebServerHandlerContainer {
	void(*handler)(OPstream*, void*);
	void* param;
};

struct OPWebServerMessagesContainer {
	ui32 messageCount;
	OPstream** messages;
};

OPWebServer* OPwebServerCreate(OPchar* port);
void OPwebServerUpdate(OPWebServer* server);
void OPwebServerOnKey(OPWebServer* server, OPchar* key, void(*handler)(OPstream*, void*), void* param);
void OPwebServerQueue(OPWebServer* server, OPchar* key, i8* data, ui32 datalen);
void OPwebServerDestroy(OPWebServer* server);

#endif
