#include "./Communication/include/OPnetwork.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"


#include "./Communication/include/OPnetworkPlatform.h"

OPnetwork* OPnetwork::Create(OPnetworkType::Enum networkType, OPnetworkProtocol::Enum protocol) {
 	OPnetwork* network = (OPnetwork*)OPalloc(sizeof(OPnetwork));
 	network->connectedSocket = (i32)INVALID_SOCKET;
 	network->networkType = networkType;
	network->networkProtocol = protocol;
	network->networkData.clientIndex = 0;
 	return network;
}

i32 OPnetwork::GetAddress(const OPchar* address, const OPchar* port, OPnetworkAddress* networkAddress, OPnetworkProtocol::Enum protocol) {
	struct addrinfo hints;

	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = protocol == OPnetworkProtocol::OPNETWORK_TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	OPchar* addr;
	ui64 r = LookupAddress(address, &addr);
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

i32 OPnetwork::Startup() {
#ifdef OPIFEX_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		OPlogDebug("WSAStartup failed.");
		return -1;
	}
#endif
	return 0;
}

void OPnetwork::LogError(const OPchar* message) {
#ifdef OPIFEX_WINDOWS
	OPlogDebug("%s: %ld", message, WSAGetLastError());
#else
	OPlogDebug("%s", message);
#endif
}

i32 OPnetwork::ClientConnect(const OPchar* address, const OPchar* port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (Startup() != 0) {
		return -1;
	}


	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
	i32 iResult = GetAddress(address, port, &networkAddress, networkProtocol);
	if (iResult != 0) {
		LogError("Error getting address");
		NETWORK_CLEANUP();
		return -2;
	}


	// Create the actual socket
 	connectedSocket = (i32)socket(networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
 	if (connectedSocket == INVALID_SOCKET) {
		LogError("Error at socket()");
 		freeaddrinfo(networkAddress.result);
		NETWORK_CLEANUP();
 		return -3;
 	}

	struct addrinfo hints, *servinfo, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = networkProtocol == OPnetworkProtocol::OPNETWORK_TCP ? SOCK_STREAM : SOCK_DGRAM;;
	if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
		OPlog("getaddrinfo: %s\n", gai_strerror(rv));
		NETWORK_CLEANUP();
		return -4;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((connectedSocket = (i32)socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			LogError("Error at socket()");
			continue;
		}
		// setsockopt(network->connectedSocket, SOL_SOCKET, SO_REUSEADDR, yes, sizeof(i32));

		if (connect(connectedSocket, p->ai_addr, (int)p->ai_addrlen) == -1) {
			LogError("Error at connect()");

			CLOSESOCKET(connectedSocket);
			OPlog("error client: connect");
			continue;
		}
	}

	FD_ZERO(&networkData.master);
	FD_ZERO(&networkData.read_fds);
	FD_SET(connectedSocket, &networkData.master);
	networkData.fdmax = connectedSocket;

 	OPlogInfo("Client connected to %s on port %s", address, port);

 	return 0;
 }

i32 OPnetwork::ServerStartTCP(OPnetwork* network) {

	 // listen() is only for TCP not UDP
	 if(listen(network->connectedSocket, 64)){
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

i32 OPnetwork::ServerStartUDP(OPnetwork* network) {
	return 0;
}

i32 OPnetwork::ServerStart(const OPchar* port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (Startup() != 0) {
		return -1;
	}

	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
	i32 iResult = GetAddress("127.0.0.1", port, &networkAddress, networkProtocol);
	if (iResult != 0) {
		LogError("Error getting address");
		NETWORK_CLEANUP();
		return -2;
	}

	OPlogDebug("Connect Socket %d, %d, %d", networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
	connectedSocket = (i32)socket(networkAddress.result->ai_family, networkAddress.result->ai_socktype, networkAddress.result->ai_protocol);
	if (connectedSocket == INVALID_SOCKET) {
		LogError("Error at socket()");
		freeaddrinfo(networkAddress.result);
		NETWORK_CLEANUP();
		return -3;
	}

	OPlogDebug("Bind Socket");
	iResult = bind(connectedSocket, networkAddress.result->ai_addr, (i32)networkAddress.result->ai_addrlen);
	freeaddrinfo(networkAddress.result);

	if (iResult == SOCKET_ERROR) {
		LogError("Failed to bind socket");
		CLOSESOCKET(connectedSocket);
		NETWORK_CLEANUP();
		return -4;
	}
	else{
		FD_ZERO(&networkData.master);
		FD_ZERO(&networkData.read_fds);
		FD_SET(connectedSocket, &networkData.master);
		networkData.fdmax = connectedSocket;

		if (networkProtocol == OPnetworkProtocol::OPNETWORK_TCP) {
			if (ServerStartTCP(this) != 0) {
				LogError("Failed to start TCP");
				return -5;
			}
		}
	}

	OPlogInfo("Server started on port %s", port);

	return 0;
}

i32 OPnetwork::ReceiveServer(void* state) {

	i8 buf[BUFSIZE];
	i32 nbytes;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	i32 i;
	i32 newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

	networkData.read_fds = networkData.master;
	if (select(networkData.fdmax + 1, &networkData.read_fds, NULL, NULL, &tv) == -1) {
		OPlog("Error with select()");
		return -1;
	}

	for (i = 0; i <= networkData.fdmax; i++) {
		if (!FD_ISSET(i, &networkData.read_fds)) {
			continue;
		}

		// This socket is ready

		if (networkProtocol == OPnetworkProtocol::OPNETWORK_TCP) {
			if (i == connectedSocket) {
				// New connection
				addrlen = sizeof remoteaddr;
				struct sockaddr* tmp = (struct sockaddr *)&remoteaddr;

				newfd = (i32)accept(connectedSocket,
					tmp,
					&addrlen
					);
				if (newfd == -1) {
					LogError("accept() failed");
					OPlog("Failed to Accept Client %s", tmp->sa_data);
				}
				else {
					OPlog("CLIENT CONNECTED");
					FD_SET(newfd, &networkData.master);
					if (newfd > networkData.fdmax) {
						networkData.fdmax = newfd;
					}
					connected(state, i);
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
					FD_CLR(i, &networkData.master); // remove from master set
					disconnected(state, i);
				}
				else {
					OPlog("Data Received From Client");
					buf[nbytes] = NULL;
					receive(state, i, nbytes, buf);
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
				FD_CLR(i, &networkData.master); // remove from master set
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
				for (; i < networkData.clientIndex; i++) {
					if (OPstringEquals(host, networkData.peer_host[i]) &&
						OPstringEquals(port, networkData.peer_port[i])) {
						OPlog("Found existing");
						found = 1;
						break;
					}
				}

				if (!found) {
					OPlog("NEW CLIENT");
					networkData.peer_addr[networkData.clientIndex] = peer_addr;
					networkData.peer_host[networkData.clientIndex] = OPstringCopy(host);
					networkData.peer_port[networkData.clientIndex] = OPstringCopy(port);
					connected(state, networkData.clientIndex);
					networkData.clientIndex++;
				}

				OPlog("Calling Receive");
				buf[nbytes] = NULL;
				receive(state, i, nbytes, buf);
			}
		}
	}

	return 0;
}

i32 OPnetwork::ReceiveClient(void* state) {

	i8 buf[BUFSIZE];
	i32 nbytes;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	i32 i;
	i32 newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

	networkData.read_fds = networkData.master;
	if (select(networkData.fdmax + 1, &networkData.read_fds, NULL, NULL, &tv) == -1) {
		LogError("Error with select()");
		return -1;
	}

	for (i = 0; i <= networkData.fdmax; i++) {
		if (networkProtocol == OPnetworkProtocol::OPNETWORK_TCP) {
			if (!FD_ISSET(i, &networkData.read_fds)) {
				continue;
			}
		}

		if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
			if (nbytes == 0) {
				OPlogErr("Connection Closed %d", i);
			} else {
				OPlogErr("Error receiving data on client");
			}
			CLOSESOCKET(i);
			FD_CLR(i, &networkData.master); // remove from master set
		}
		else {
			OPlog("Data Received From Client");
			buf[nbytes] = NULL;
			receive(state, i, nbytes, buf);
		}
	}

	return 0;
}

i32 OPnetwork::Receive(void* state) {

	if (networkType == OPnetworkType::OPNETWORK_SERVER) {
		ReceiveServer(state);
	}
	else {
		ReceiveClient(state);
	}

return 0;
}

i32 OPnetwork::Send(i8* data, i32 size){

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

	if (networkType == OPnetworkType::OPNETWORK_SERVER) {

		if (networkProtocol == OPnetworkProtocol::OPNETWORK_TCP) {
			for (j = 0; j <= networkData.fdmax; j++) {
				// send to everyone!
				if (FD_ISSET(j, &networkData.master)) {
					// except the listener and ourselves
					if (j != connectedSocket) {
						if (send(j, data, size, 0) == -1) {
							OPlog("Send error");
						}
					}
				}
			}
		}
		else {
			socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
			for (i32 i = 0; i < networkData.clientIndex; i++) {
				if (sendto(connectedSocket, data, size, 0, (struct sockaddr *)&networkData.peer_addr[i], peer_addr_len) == -1) {
					LogError("send error");
				}
			}
		}
	}
	else {
		if (send(connectedSocket, data, size, 0) == -1) {
			LogError("send error");
		}
	}

	return 0;
}

i32 OPnetwork::Shutdown(){
	i32 iResult = shutdown(connectedSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		LogError("shutdown failed");
		return -1;
	}

	CLOSESOCKET(connectedSocket);
	NETWORK_CLEANUP();

	return 0;
}

void OPnetwork::Destroy() {
	
}


ui64 OPnetwork::LookupAddress(const OPchar *kpAddress, OPchar** resolved)
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