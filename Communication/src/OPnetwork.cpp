 #include "./Communication/include/OPnetwork.h"
 #include "./Core/include/OPlog.h"
 #include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

 #ifndef OPIFEX_ANDROID

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

 #ifdef OPIFEX_WINDOWS
	 #include <winsock2.h>
	 #include <ws2tcpip.h>
	 #include <iphlpapi.h>
	 #pragma comment(lib, "Ws2_32.lib")
	 #define NETWORK_CLEANUP() { WSACleanup(); }
	#define CLOSESOCKET(socket) closesocket(socket);
 #endif

 #ifdef OPIFEX_UNIX
	 #include <arpa/inet.h>
	 #include <sys/types.h>
	 #include <sys/socket.h>
	 #include <netdb.h>
	 #include <unistd.h>

	 #define NETWORK_CLEANUP(){ }
	 #define INVALID_SOCKET -1
	 #define SOCKET_ERROR -1
	 #define SD_SEND SHUT_WR

	#define CLOSESOCKET(socket) close(socket);
 #endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#define BUFSIZE 2048

struct OPnetworkAddress {
	struct addrinfo* result;
	i32 iResult;
};


ui64 OPnetworkLookupAddress(OPchar *kpAddress, OPchar** resolved)
 {
 	ui64 a;
	*resolved = NULL;

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

 		i32 i = 0;
 		struct in_addr addr;

 		OPlog("Network: %s\n", pHE->h_name);
 		if (pHE->h_addrtype == AF_INET)
 		{
 			while (pHE->h_addr_list[i] != 0) {
 				addr.s_addr = *(u_long *)pHE->h_addr_list[i++];

 				OPchar* ad = inet_ntoa(addr);
 				ui32 len = (ui32)strlen(ad);
 				*resolved = (OPchar*)OPalloc(len + 1);
 				OPmemcpy(*resolved, ad, len);
 				(*resolved)[len] = NULL;

 				OPlog("IP Address %s\n", ad);
 			}
 		}
	}

 	return a;
 }

OPnetwork* OPnetworkCreate(OPnetworkType networkType, OPnetworkPrototcol protocol) {
 	OPnetwork* network = (OPnetwork*)OPalloc(sizeof(OPnetwork));
 	network->ConnectSocket = INVALID_SOCKET;
 	network->ConnectionType = networkType;
	network->ConnectionProtocol = protocol;
	network->Data.clientIndex = 0;
 	return network;
}

i32 OPnetworkGetAddress(OPchar* address, OPchar* port, OPnetworkAddress* networkAddress, OPnetworkPrototcol protocol) {
	struct addrinfo hints;

	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = protocol == OPNETWORK_TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	OPchar* addr;
	ui64 r = OPnetworkLookupAddress(address, &addr);
	OPlogDebug("Address %d:%s", r, port);
	networkAddress->iResult = getaddrinfo(addr == NULL ? address : addr, port, &hints, &networkAddress->result);
	if (addr != NULL) {
		OPfree(addr);
	}

	if (networkAddress->iResult != 0) {
		OPlog("getaddrinfo failed: %d", networkAddress->iResult);

		if (networkAddress->iResult == EAI_AGAIN) {
			OPlog("EAI_AGAIN");
		}
		if (networkAddress->iResult == EAI_BADFLAGS) {
			OPlog("EAI_BADFLAGS");
		}
		if (networkAddress->iResult == EAI_FAIL) {
			OPlog("EAI_FAIL");
		}
		if (networkAddress->iResult == EAI_FAMILY) {
			OPlog("EAI_FAMILY");
		}
		if (networkAddress->iResult == EAI_MEMORY) {
			OPlog("EAI_MEMORY");
		}
		if (networkAddress->iResult == EAI_NONAME) {
			OPlog("EAI_NONAME");
		}
#ifndef OPIFEX_WINDOWS
		if (networkAddress->iResult == EAI_OVERFLOW) {
			OPlog("EAI_OVERFLOW");
		}
		if (networkAddress->iResult == EAI_SYSTEM) {
			OPlog("EAI_SYSTEM");
		}
#endif
		return -1;
	}

	return 0;
}

i32 OPnetworkStartup() {
#ifdef OPIFEX_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		OPlogDebug("WSAStartup failed.");
		return -1;
	}
#endif
	return 0;
}

void OPnetworkLogError(OPchar* message) {
#ifdef OPIFEX_WINDOWS
	OPlogDebug("%s: %ld", message, WSAGetLastError());
#else
	OPlogDebug("%s", message);
#endif
}

i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPchar* port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetworkStartup() != 0) {
		return -1;
	}
	 

	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
	i32 iResult = OPnetworkGetAddress(address, port, &networkAddress, network->ConnectionProtocol);
	if (iResult != 0) {
		OPnetworkLogError("Error getting address");
		NETWORK_CLEANUP();
		return -2;
	}
	
	
	// Create the actual socket
 	network->ConnectSocket = socket(networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
 	if (network->ConnectSocket == INVALID_SOCKET) {
		OPnetworkLogError("Error at socket()");
 		freeaddrinfo(networkAddress.result);
		NETWORK_CLEANUP();
 		return -3;
 	}

	struct addrinfo hints, *servinfo, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = network->ConnectionProtocol == OPNETWORK_TCP ? SOCK_STREAM : SOCK_DGRAM;;
	if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
		OPlog("getaddrinfo: %s\n", gai_strerror(rv));
		NETWORK_CLEANUP();
		return -4;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((network->ConnectSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			OPnetworkLogError("Error at socket()");
			continue;
		}
		// setsockopt(network->ConnectSocket, SOL_SOCKET, SO_REUSEADDR, yes, sizeof(i32));
	
		if (connect(network->ConnectSocket, p->ai_addr, p->ai_addrlen) == -1) {
			OPnetworkLogError("Error at connect()");
	
			CLOSESOCKET(network->ConnectSocket);
			OPlog("error client: connect");
			continue;
		}
	}

	FD_ZERO(&network->Data.master);
	FD_ZERO(&network->Data.read_fds);
	FD_SET(network->ConnectSocket, &network->Data.master);
	network->Data.fdmax = network->ConnectSocket;

 	OPlogInfo("Client connected to %s on port %s", address, port);

 	return 0;
 }

i32 OPnetworkServerStartTCP(OPnetwork* network) {

	 // listen() is only for TCP not UDP	
	 if(listen(network->ConnectSocket, 64)){
	 	const OPchar* errMsg = "No Error";
	 	switch(errno){
	 		case EACCES:
	 			errMsg = "The current process has insufficient privileges.";
	 			break;
	 		case EBADF:
	 			errMsg = "The argument socket is not a valid file descriptor.";
	 			break;
	 		case EDESTADDRREQ:
	 			errMsg = "The socket is not bound to a local address and the protocol does not support listening on an unbound socket.";
	 			break;
	 		case EINVAL:
	 			errMsg = "socket is already connected.";
	 			break;
	 		case ENOTSOCK:
	 			errMsg = "The argument socket does not reference a socket.";
	 			break;
	 		case EOPNOTSUPP:
	 			errMsg = "The socket is not of a type that supports the operation listen().";
	 			break;
	 	}
	 	OPlog("Error on listen()\n\t- %s", errMsg);
	 }
	return 0;
}

i32 OPnetworkServerStartUDP(OPnetwork* network) {
	return 0;
}

 i32 OPnetworkServerStart(OPnetwork* network, OPchar* port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetworkStartup() != 0) {
		return -1;
	}

	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
	i32 iResult = OPnetworkGetAddress("127.0.0.1", port, &networkAddress, network->ConnectionProtocol);
	if (iResult != 0) {
		OPnetworkLogError("Error getting address");
		NETWORK_CLEANUP();
		return -2;
	}

 	OPlogDebug("Connect Socket %d, %d, %d", networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
 	network->ConnectSocket = socket(networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
	if (network->ConnectSocket == INVALID_SOCKET) {
		OPnetworkLogError("Error at socket()");
		freeaddrinfo(networkAddress.result);
		NETWORK_CLEANUP();
		return -3;
	}

 	OPlogDebug("Bind Socket");
 	iResult = bind(network->ConnectSocket, networkAddress.result->ai_addr, (i32)networkAddress.result->ai_addrlen);
	freeaddrinfo(networkAddress.result);

 	if (iResult == SOCKET_ERROR) {
		OPnetworkLogError("Failed to bind socket");
		CLOSESOCKET(network->ConnectSocket);
		NETWORK_CLEANUP();
 		return -4;
 	}
 	else{
		FD_ZERO(&network->Data.master);
		FD_ZERO(&network->Data.read_fds);
		FD_SET(network->ConnectSocket, &network->Data.master);
		network->Data.fdmax = network->ConnectSocket;

		if (network->ConnectionProtocol == OPNETWORK_TCP) {
			if (OPnetworkServerStartTCP(network) != 0) {
				OPnetworkLogError("Failed to start TCP");
				return -5;
			}
		}
 	}

 	OPlogInfo("Server started on port %s", port);

 	return 0;
 }



 i32 OPnetworkReceive(OPnetwork* network, void* state) {

// 	OPchar recvbuf[DEFAULT_BUFLEN];
// 	i32 recvbuflen = DEFAULT_BUFLEN;
// 	i32 iResult = 0;
//	sockaddr si_other;
//
//#ifdef OPIFEX_WINDOWS
//	i32 si_len = sizeof(si_other);
//#else
//	ui32 si_len = sizeof(si_other);
//#endif
//
// 	fd_set readfds;
//
//
// 	//FD_SET(sd, &readfds);
//
// 	fflush(stdout);
// 	memset(recvbuf, '\0', DEFAULT_BUFLEN);
//
//
// 	iResult = recvfrom(network->ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0, &si_other, &si_len);
// 	if(iResult < 0) {
// 		OPlog("Recv Error");
//
// 		switch(errno) {
// 			case EWOULDBLOCK:
// 				OPlog("EWOULDBLOCK / EAGAIN");
// 				break;
// 			case ECONNREFUSED:
// 				OPlog("ECONNREFUSED");
// 				break;
// 			case EINTR:
// 				OPlog("EINTR");
// 				break;
// 			case EINVAL:
// 				OPlog("EINVAL");
// 				break;
// 			case ENOMEM:
// 				OPlog("ENOMEM");
// 				break;
// 			case ENOTCONN:
// 				OPlog("ENOTCONN");
// 				break;
// 			case ENOTSOCK:
// 				OPlog("ENOTSOCK");
// 				break;
// 			default:
// 				//OPlog("NO ERROR MATCH %d", errno);
// 				break;
// 		}
// 	} else if(iResult > 0) {
// 		OPlog("Bytes received: %d %d\n", iResult, *(int*)(recvbuf));
// 	}
//
 	// Receive until the peer shuts down the connection
 	// do {
 		// iResult = recv(network->ConnectSocket, recvbuf, recvbuflen, 0);
 		// if(iResult > 0) {
 		// 	OPlog("Bytes received: %d %d\n", iResult, *(int*)(recvbuf));
 		// }
 	// } while (true);
 	//OPlog("Receiving Message...");
 	//select()
 	// if ((iResult = recvfrom(network->ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0, &si_other, &si_len)) == SOCKET_ERROR) {
 	// 	return 1;
 	// }

 	//receive(state, iResult, recvbuf);


	 i8 buf[BUFSIZE];
	 i32 nbytes;

	 struct timeval tv;
	 tv.tv_sec = 0;
	 tv.tv_usec = 1000;

	 i32 i;
	 i32 newfd;
	 struct sockaddr_storage remoteaddr;
	 socklen_t addrlen;

	 if (network->ConnectionType == OPNETWORK_SERVER) {

		 network->Data.read_fds = network->Data.master;
		 if (select(network->Data.fdmax + 1, &network->Data.read_fds, NULL, NULL, &tv) == -1) {
			 OPlog("Error with select()");
			 return -1;
		 }

		 for (i = 0; i <= network->Data.fdmax; i++) {
			 if (!FD_ISSET(i, &network->Data.read_fds)) {
				 continue;
			 }

			 // This socket is ready

			 if (network->ConnectionProtocol == OPNETWORK_TCP) {
				 if (i == network->ConnectSocket) {
					 // New connection
					 addrlen = sizeof remoteaddr;
					 struct sockaddr* tmp = (struct sockaddr *)&remoteaddr;

					 newfd = accept(network->ConnectSocket,
						 tmp,
						 &addrlen
						 );
					 if (newfd == -1) {
						 OPnetworkLogError("accept() failed");
						 OPlog("Failed to Accept Client %s", tmp->sa_data);
					 }
					 else {
						 OPlog("CLIENT CONNECTED");
						 FD_SET(newfd, &network->Data.master);
						 if (newfd > network->Data.fdmax) {
							 network->Data.fdmax = newfd;
						 }
						 network->connected(state, i);
						 // OPlog("New connection from %s on socket %d",
						 // 	inet_ntop(remoteaddr.ss_family,
						 // 	get_in_addr((struct sockaddr*)&remoteaddr),
						 // 	remoteIP, INET6_ADDRSTRLEN),
						 // 	newfd
						 // );
					 }
				 }
				 else {
					 // Data from client
					 if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
						 if (nbytes == 0) {
							 OPlog("Connection Closed %d", i);
						 }
						 else {
							 OPlog("Error receiving data");
						 }
						 CLOSESOCKET(i);
						 FD_CLR(i, &network->Data.master); // remove from master set
						 network->disconnected(state, i);
					 }
					 else {
						 OPlog("Data Received From Client");
						 buf[nbytes] = NULL;
						 network->receive(state, i, nbytes, buf);
					 }
				 }
			 }
			 else {

				 socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
				 int s;
				 struct sockaddr_storage peer_addr;

				 // Data from client
				 if ((nbytes = recvfrom(i, buf, sizeof buf, 0,
					 (struct sockaddr *) &peer_addr, &peer_addr_len)) <= 0) {
					 if (nbytes == 0) {
						 OPlog("Connection Closed %d", i);
					 }
					 else {
						 OPlog("Error receiving data");
					 }
					 CLOSESOCKET(i);
					 FD_CLR(i, &network->Data.master); // remove from master set
				 }
				 else {
					 OPlog("Data Received From Client");
					 char host[NI_MAXHOST], port[NI_MAXSERV];

					 s = getnameinfo((struct sockaddr *) &peer_addr,
						 peer_addr_len, host, NI_MAXHOST,
						 port, NI_MAXSERV, NI_NUMERICSERV);

					 if (s == 0) {
						 OPlog("Received %zd bytes from %s:%s\n", nbytes, host, port);
					 } else {
						 OPlogErr("getnameinfo: %s\n", gai_strerror(s));
						 OPlog("Received %zd bytes from %s:%s\n", nbytes, host, port);
					 }

					 i32 found = 0;
					 i32 i = 0;
					 for (; i < network->Data.clientIndex; i++) {
						 if (OPstringEquals(host, network->Data.peer_host[i]) &&
							 OPstringEquals(port, network->Data.peer_port[i])) {
							 OPlog("Found existing");
							 found = 1;
							 break;
						 }
					 }

					 if (!found) {
						 OPlog("NEW CLIENT");
						 network->Data.peer_addr[network->Data.clientIndex] = peer_addr;
						 network->Data.peer_host[network->Data.clientIndex] = OPstringCopy(host);
						 network->Data.peer_port[network->Data.clientIndex] = OPstringCopy(port);
						 network->connected(state, network->Data.clientIndex);
						 network->Data.clientIndex++;
					 }

					 OPlog("Calling Receive");
					 buf[nbytes] = NULL;
					 network->receive(state, i, nbytes, buf);
				 }
			 }
		 }

		 return 0;
	 }
	 else {

		 network->Data.read_fds = network->Data.master;
		 if (select(network->Data.fdmax + 1, &network->Data.read_fds, NULL, NULL, &tv) == -1) {
			 OPnetworkLogError("Error with select()");
			 return -1;
		 }

		 for (i = 0; i <= network->Data.fdmax; i++) {
			 if (network->ConnectionType == OPNETWORK_TCP) {
				 if (!FD_ISSET(i, &network->Data.read_fds)) {
					 continue;
				 }
			 }

			 if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
				 if (nbytes == 0) {
					 OPlog("Connection Closed %d", i);
				 } else {
					 OPlog("Error receiving data");
				 }
				 CLOSESOCKET(i);
				 FD_CLR(i, &network->Data.master); // remove from master set
			 }
			 else {
				 OPlog("Data Received From Client");
				 buf[nbytes] = NULL;
				 network->receive(state, i, nbytes, buf);
			 }
		 }

		 return 0;
	 }

	return 0;
}

 i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size){

 	//OPchar recvbuf[DEFAULT_BUFLEN];
 	//i32 recvbuflen = DEFAULT_BUFLEN;
 	//struct addrinfo *result = NULL, *ptr = NULL, hints;
 	//OPbzero(&hints, sizeof (hints));
 	//hints.ai_family = AF_INET;
 	//hints.ai_socktype = SOCK_DGRAM;
 	//hints.ai_protocol = IPPROTO_UDP;
 	//hints.ai_flags = AI_PASSIVE;
 	//i32 iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
 	//if (iResult != 0) {
 	//	OPlog("getaddrinfo for loop-back failed: %d", iResult);
 	//	NETWORK_CLEANUP();
 	//	return 1;
 	//}
 	//i32 slen = sizeof(*result->ai_addr);
 	//OPlogDebug("Sending Message...");
 	////send(network->ConnectSocket, recvbuf, iResult, 0);
 	////return 1;
 	//if (sendto(network->ConnectSocket, data, size, 0, result->ai_addr, slen) == SOCKET_ERROR) {
 	//	return 1;
 	//}

	int j;

	if (network->ConnectionType == OPNETWORK_SERVER) {

		if (network->ConnectionProtocol == OPNETWORK_TCP) {
			for (j = 0; j <= network->Data.fdmax; j++) {
				// send to everyone!
				if (FD_ISSET(j, &network->Data.master)) {
					// except the listener and ourselves
					if (j != network->ConnectSocket) {
						if (send(j, data, size, 0) == -1) {
							OPlog("Send error");
						}
					}
				}
			}
		}
		else {
			socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
			for (i32 i = 0; i < network->Data.clientIndex; i++) {
				if (sendto(network->ConnectSocket, data, size, 0, (struct sockaddr *)&network->Data.peer_addr[i], peer_addr_len) == -1) {
					OPnetworkLogError("send error");
				}
			}
		}
	}
	else {
		if (send(network->ConnectSocket, data, size, 0) == -1) {
			OPnetworkLogError("send error");
		}
	}

 	return 0;
 }

 i32 OPnetworkShutdown(OPnetwork* network){
 	i32 iResult = shutdown(network->ConnectSocket, SD_SEND);
 	if (iResult == SOCKET_ERROR) {
		OPnetworkLogError("shutdown failed");
 		return -1;
 	}

 	CLOSESOCKET(network->ConnectSocket);
	NETWORK_CLEANUP();

 	return 0;
 }

 void OPnetworkDestroy(OPnetwork* network) {
 	OPfree(network);
 }

 #endif