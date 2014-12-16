#ifndef OPENGINE_COMMUNICATION_WEBSERVER
#define OPENGINE_COMMUNICATION_WEBSERVER

#include "./Core/include/Types.h"
#include "./Communication/include/Mongoose.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"

typedef struct {
	struct mg_server* Server;
	// WebSocket Messages
	// Key, Data, Data Length
	void(*Receive)(i8*, i8*, i32);
	OPhashMap* WebSocketKeys;
	OPlist* WebSocketMessages;
} OPWebServer;

typedef struct {
	void(*handler)(OPstream*, void*);
	void* param;
} OPWebServerHandlerContainer;

typedef struct {
	ui32 messageCount;
	OPstream** messages;
} OPWebServerMessagesContainer;

OPWebServer* OPwebServerCreate(OPchar* port);
void OPwebServerUpdate(OPWebServer* server);
void OPwebServerOnKey(OPWebServer* server, OPchar* key, void(*handler)(OPstream*, void*), void* param);
void OPwebServerQueue(OPWebServer* server, OPchar* key, i8* data, ui32 datalen);
void OPwebServerDestroy(OPWebServer* server);

#endif