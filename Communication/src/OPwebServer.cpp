
#include "./Communication/include/OPwebServer.h"

#ifndef OPIFEX_ANDROID
#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"

#include <time.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif


static int iterate_callback(struct mg_connection *c, enum mg_event ev, void* server_obj) {
	if (ev == MG_POLL && c->is_websocket) {
		OPWebServerMessagesContainer* messagesContainer = (OPWebServerMessagesContainer*)c->callback_param;
		for (i32 i = 0; i < messagesContainer->messageCount; i++) {
			mg_websocket_write(c, 2, (i8*)messagesContainer->messages[i]->Data, messagesContainer->messages[i]->Size);
		}
	}

	return MG_TRUE;
}

static int send_reply(struct mg_connection *conn, OPWebServer* server) {
	if (conn->is_websocket) {
		OPlog("Web Socket Request... %s", conn->content);
		// This handler is called for each incoming websocket frame, one or more
		// times for connection lifetime.
		// Echo websocket data back to the client.
		//i8* data = (i8*)OPalloc(conn->content_len + 1);
		if (conn->content_len == 0 && conn->wsbits == 138) {
			mg_websocket_write(conn, 10, "", 0);
			return MG_TRUE;
		}

		if (conn->wsbits == 136) {
			return MG_FALSE;
		}

		
		conn->content[conn->content_len] = '\0';

		OPstream* str = OPstreamCreate(conn->content_len);
		OPmemcpy(str->Data, conn->content, conn->content_len);

		i8* key = OPreadstring(str);
		OPlog("KEY: %s", key);

		if (OPhashMapExists(server->WebSocketKeys, key)) {
			OPWebServerHandlerContainer* container;
			void(*handler)(OPstream*);
			OPhashMapGet(server->WebSocketKeys, key, (void**)&container);
			container->handler(str, container->param);
			mg_websocket_write(conn, 2, conn->content, conn->content_len);
		}

		OPstreamDestroy(str);

		return conn->content_len == 4 && !memcmp(conn->content, "exit", 4) ?
			MG_FALSE : MG_TRUE;
		//return MG_FALSE;
	}
	else {
		OPlog("Requested: %s", conn->uri);
		i32 uriLength = strlen(conn->uri);
		if (conn->uri[0] == '/' && uriLength == 1) {
			OPstream* index = OPreadFile("Web/index.html");
			mg_send_header(conn, "Content-Type", "text/html");
			mg_send_data(conn, index->Data, index->Length);
			return MG_TRUE;
		} else {
			const OPchar* path = "";
			i32 urisize = strlen(conn->uri);
			i32 size = urisize + strlen(path);

			if (urisize < 1) return MG_TRUE;
			i32 offset = conn->uri[0] == '/';
			i8* filepath = (i8*)OPalloc(sizeof(i8)* size + 1 - offset);

			OPmemcpy(filepath, path, strlen(path));
			OPmemcpy(filepath + strlen(path), conn->uri + offset, strlen(conn->uri) - offset);
			filepath[size - offset] = '\0';


			if ((size == 12 || size == 11) && OPmemcmp(filepath + size - 4, ".ico", 4)) {
				OPstream* index = OPreadFile("Web/favicon.ico");
				mg_send_header(conn, "Content-Type", "text/html");
				mg_send_data(conn, index->Data, index->Length);
				return MG_TRUE;
			}

			OPstream* index = OPreadFile(filepath);
			if (index == NULL) return MG_TRUE;


			if (OPmemcmp(filepath + size - offset - 3, ".js", 3) == 0) {
				mg_send_header(conn, "Content-Type", "application/javascript");
			}
			else if (OPmemcmp(filepath + size - offset - 4, ".png", 4) == 0) {
				mg_send_header(conn, "Content-Type", "image/png");
			}
			else if (OPmemcmp(filepath + size - offset - 4, ".css", 3) == 0) {
				mg_send_header(conn, "Content-Type", "text/css");
			}
			else  {
				mg_send_header(conn, "Content-Type", "text/html");
			}

			OPfree(filepath);

			mg_send_data(conn, index->Data, index->Size);
			OPstreamDestroy(index);
			return MG_TRUE;
		}

		return MG_FALSE;
	}
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev, void* server_obj) {
	if (ev == MG_REQUEST) {
		return send_reply(conn, (OPWebServer*)server_obj);
	}
	else if (ev == MG_AUTH) {
		return MG_TRUE;
	}
	else {
		return MG_FALSE;
	}
}

#endif


OPWebServer* OPwebServerCreate(OPchar* port) {
	OPWebServer* server = (OPWebServer*)OPalloc(sizeof(OPWebServer));
#ifndef OPIFEX_ANDROID
	server->WebSocketKeys = OPhashMapCreate(16);
	server->WebSocketMessages = OPlistCreate(16, sizeof(OPstream));

	server->Server = mg_create_server(NULL, ev_handler, server);
	mg_set_option(server->Server, "listening_port", port);
	printf("Started on port %s\n", mg_get_option(server->Server, "listening_port"));
#endif
	return server;
}

void OPwebServerOnKey(OPWebServer* server, OPchar* key, void(*handler)(OPstream*, void*), void* param) {

#ifndef OPIFEX_ANDROID
	OPWebServerHandlerContainer* container = (OPWebServerHandlerContainer*)OPalloc(sizeof(OPWebServerHandlerContainer));
	container->handler = handler;
	container->param = param;
	OPhashMapPut(server->WebSocketKeys, key, (void*)container);
#endif
}

void OPwebServerUpdate(OPWebServer* server) {
#ifndef OPIFEX_ANDROID
	mg_poll_server(server->Server, 1);

	OPWebServerMessagesContainer* messagesContainer = (OPWebServerMessagesContainer*)OPalloc(sizeof(OPWebServerMessagesContainer));
	messagesContainer->messageCount = OPlistSize(server->WebSocketMessages);
	messagesContainer->messages = (OPstream**)OPalloc(sizeof(OPstream*) * messagesContainer->messageCount);

	for (i32 i = 0; i < messagesContainer->messageCount; i++) {
		messagesContainer->messages[i] = (OPstream*)OPlistPop(server->WebSocketMessages);;
	}

	mg_iterate_over_connections(server->Server, iterate_callback, messagesContainer);

	//for (i32 i = 0; i < messagesContainer->messageCount; i++) {
	//	OPstreamDestroy(messagesContainer->messages[i]);
	//}

	OPfree(messagesContainer->messages);
	OPfree(messagesContainer);
#endif
}

void OPwebServerQueue(OPWebServer* server, OPchar* key, i8* data, ui32 datalen) {
#ifndef OPIFEX_ANDROID
	ui32 size = sizeof(i8) * strlen(key) + datalen;
	OPstream* d = OPstreamCreate(size);
	
	ui32 len = strlen(key);
	OPwrite(d, (ui8*)&len, sizeof(ui32));
	OPwrite(d, key, sizeof(OPchar)* len);
	OPwrite(d, data, datalen);
	OPseek(d, 0);
	OPlistPush(server->WebSocketMessages, (ui8*)d);
#endif
}

void OPwebServerDestroy(OPWebServer* server) {

#ifndef OPIFEX_ANDROID
	OPlistDestroy(server->WebSocketMessages);
	OPfree(server->WebSocketMessages);
	OPhashMapDestroy(server->WebSocketKeys);
	OPfree(server->WebSocketKeys);
	mg_destroy_server(&server->Server);
#endif
	OPfree(server);
}