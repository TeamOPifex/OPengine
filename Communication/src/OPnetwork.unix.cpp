#include "./Communication/include/OPnetwork.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

#ifdef OPIFEX_UNIX

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define INVALID_SOCKET -1
#define BUFSIZE 2048

ui64 OPnetworkLookupAddress(const OPchar *kpAddress, OPchar** resolved)
{
	ui64 a;

	OPlog("OPnetworkLookupAddress() - Resolving '%s'", kpAddress);

	if ((a = inet_addr(kpAddress)) == INADDR_NONE)
	{
		hostent* pHE = gethostbyname(kpAddress);

		if (pHE == 0)
		{
			OPlog("FAILED TO RESOLVE ADDRESS");
			// Failed to resolve the address
			return INADDR_NONE;
		}

		OPchar** pAlias;
		i32 i = 0;
		struct in_addr addr;

		OPlog("Network: %s\n", pHE->h_name);
		if (pHE->h_addrtype == AF_INET)
		{
			while (pHE->h_addr_list[i] != 0) {
				addr.s_addr = *(u_long *)pHE->h_addr_list[i++];

				OPchar* ad = inet_ntoa(addr);
				i32 len = strlen(ad);
				*resolved = (OPchar*)OPalloc(len + 1);
				OPmemcpy(*resolved, ad, len);
				(*resolved)[len] = NULL;

				OPlog("IP Address %s\n", ad);
			}
		}
	}

	return a;
}

OPnetwork* OPnetworkCreate(OPnetworkType networkType) {
	OPnetwork* network = (OPnetwork*)OPalloc(sizeof(OPnetwork));
	network->ConnectSocket = INVALID_SOCKET;
	network->ConnectionType = networkType;
	return network;
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPchar* port) {

	network->Data = OPalloc(sizeof(OPneworkServerData));
	OPneworkServerData* serverData = (OPneworkServerData*)network->Data;

	FD_ZERO(&serverData->master);
	FD_ZERO(&serverData->read_fds);

	int numbytes;
	char buf[BUFSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
		OPlog("getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((network->ConnectSocket = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			OPlog("error client: socket");
			continue;
		}

		if (connect(network->ConnectSocket, p->ai_addr, p->ai_addrlen) == -1) {
			close(network->ConnectSocket);
			OPlog("error client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	OPlog("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	FD_SET(network->ConnectSocket, &serverData->master);
	serverData->fdmax = network->ConnectSocket;

	return 0;
}

i32 OPnetworkServerStart(OPnetwork* network, OPchar* port) {

	network->Data = OPalloc(sizeof(OPneworkServerData));
	OPneworkServerData* serverData = (OPneworkServerData*)network->Data;


	// VARIABLES FOR SETTING UP THE SOCKET

	i8 remoteIP[INET6_ADDRSTRLEN];

	i32 yes = 1;
	i32 rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&serverData->master);
	FD_ZERO(&serverData->read_fds);


	// GETTING THE ACTUAL SOCKET

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Can choose either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP Server
	hints.ai_flags = AI_PASSIVE; // Auto choose this host
	if((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0) {
		OPlog("Error Getting Address Info");
		return -1;
	}

	// Loop through each IP Address until one works
	for(p = ai; p != NULL; p = p->ai_next) {
		network->ConnectSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(network->ConnectSocket < 0) {
			continue;
		}

		// This gets rid of the "address already in use" error msg
		setsockopt(network->ConnectSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(i32));

		if(bind(network->ConnectSocket, p->ai_addr, p->ai_addrlen) < 0) {
			close(network->ConnectSocket);
			continue;
		}

		break;
	}
	freeaddrinfo(ai);

	if(p == NULL) {
		// If we got here, none of the results were able to bind
		OPlog("Unable to bind");
		return -2;
	}

	if(listen(network->ConnectSocket, 10) == -1) {
		OPlog("Unable to listen");
		return -3;
	}

	FD_SET(network->ConnectSocket, &serverData->master);
	serverData->fdmax = network->ConnectSocket;

	OPlog("Server Started on %s", port);

	return 0;
}

i32 OPnetworkReceive(OPnetwork* network, void* state, void(*receive)(void*, i32, OPchar*)) {
	i8 buf[BUFSIZE];
	i32 nbytes;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	i32 i, j;
	i32 newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

	if(network->ConnectionType == OPNETWORK_SERVER) {

		OPneworkServerData* serverData = (OPneworkServerData*)network->Data;

		serverData->read_fds = serverData->master;
		if(select(serverData->fdmax + 1, &serverData->read_fds, NULL, NULL, &tv) == -1) {
			OPlog("Error with select()");
			return -1;
		}

		for(i = 0; i <= serverData->fdmax; i++) {
			if(!FD_ISSET(i, &serverData->read_fds)) {
				continue;
			}

			// This socket is ready

			if(i == network->ConnectSocket) {
				// New connection
				addrlen = sizeof remoteaddr;
				newfd = accept(network->ConnectSocket,
					(struct sockaddr *)&remoteaddr,
					&addrlen
				);
				if(newfd == -1) {
					OPlog("Failed to Accept Client");
				} else {
					OPlog("CLIENT CONNECTED");
					FD_SET(newfd, &serverData->master);
					if(newfd > serverData->fdmax) {
						serverData->fdmax = newfd;
					}
					// OPlog("New connection from %s on socket %d",
					// 	inet_ntop(remoteaddr.ss_family,
					// 	get_in_addr((struct sockaddr*)&remoteaddr),
					// 	remoteIP, INET6_ADDRSTRLEN),
					// 	newfd
					// );
				}
			} else {
				// Data from client
				if((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
					if(nbytes == 0) {
						OPlog("Connection Closed %d", i);
					} else {
						OPlog("Error receiving data");
					}
                    close(i); // bye!
                    FD_CLR(i, &serverData->master); // remove from master set
				} else {
					OPlog("Data Received From Client");
					receive(state, 1, buf);
				}
			}
		}

		return 0;
	} else {

		OPneworkServerData* clientData = (OPneworkServerData*)network->Data;

		clientData->read_fds = clientData->master;
		if(select(clientData->fdmax + 1, &clientData->read_fds, NULL, NULL, &tv) == -1) {
			OPlog("Error with select()");
			return -1;
		}

		for(i = 0; i <= clientData->fdmax; i++) {
			if(!FD_ISSET(i, &clientData->read_fds)) {
				continue;
			}
			if((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
				if(nbytes == 0) {
					OPlog("Connection Closed %d", i);
				} else {
					OPlog("Error receiving data");
				}
				close(i); // bye!
				FD_CLR(i, &clientData->master); // remove from master set
			} else {
				OPlog("Data Received From Client");
				receive(state, 1, buf);
			}
		}

		return 0;
	}
}

i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size){
	int j;

	if(network->ConnectionType == OPNETWORK_SERVER) {

		OPneworkServerData* serverData = (OPneworkServerData*)network->Data;

		for(j = 0; j <= serverData->fdmax; j++) {
			// send to everyone!
			if (FD_ISSET(j, &serverData->master)) {
				// except the listener and ourselves
				if (j != network->ConnectSocket) {
					if (send(j, data, size, 0) == -1) {
						OPlog("Send error");
					}
				}
			}
		}
	} else {
		if (send(network->ConnectSocket, data, size, 0) == -1) {
			OPlog("send error");
		}
	}

	return 0;
}

i32 OPnetworkShutdown(OPnetwork* network){

	return 0;
}

void OPnetworkDestroy(OPnetwork* network) {
	OPfree(network);
}

#endif
