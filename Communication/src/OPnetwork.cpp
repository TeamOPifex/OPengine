// #include "./Communication/include/OPnetwork.h"
// #include "./Core/include/OPlog.h"
// #include "./Core/include/OPmemory.h"
//
// #ifndef OPIFEX_ANDROID
//
// #ifdef OPIFEX_WINDOWS
// #include <windows.h>
// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <iphlpapi.h>
// #include <stdio.h>
// #pragma comment(lib, "Ws2_32.lib")
// #define WSA_CLEANUP() {WSACleanup();}
// #endif
//
// #ifdef OPIFEX_UNIX
// #include <arpa/inet.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <unistd.h>
//
// #define WSA_CLEANUP(){ }
// #define INVALID_SOCKET -1
// #define SOCKET_ERROR -1
// #define SD_SEND SHUT_WR
//
// #endif
//
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
//
//
//
// ui64 OPnetworkLookupAddress(const OPchar *kpAddress, OPchar** resolved)
// {
// 	ui64 a;
//
// 	OPlog("OPnetworkLookupAddress() - Resolving '%s'", kpAddress);
//
// 	if ((a = inet_addr(kpAddress)) == INADDR_NONE)
// 	{
// 		hostent* pHE = gethostbyname(kpAddress);
//
// 		if (pHE == 0)
// 		{
// 			OPlog("FAILED TO RESOLVE ADDRESS");
// 			// Failed to resolve the address
// 			return INADDR_NONE;
// 		}
//
// 		OPchar** pAlias;
// 		i32 i = 0;
// 		struct in_addr addr;
//
// 		OPlog("Network: %s\n", pHE->h_name);
// 		if (pHE->h_addrtype == AF_INET)
// 		{
// 			while (pHE->h_addr_list[i] != 0) {
// 				addr.s_addr = *(u_long *)pHE->h_addr_list[i++];
//
// 				OPchar* ad = inet_ntoa(addr);
// 				i32 len = strlen(ad);
// 				*resolved = (OPchar*)OPalloc(len + 1);
// 				OPmemcpy(*resolved, ad, len);
// 				(*resolved)[len] = NULL;
//
// 				OPlog("IP Address %s\n", ad);
// 			}
// 		}
// 	}
//
// 	return a;
// }
//
// OPnetwork* OPnetworkCreate(OPnetworkType networkType) {
// 	OPnetwork* network = (OPnetwork*)OPalloc(sizeof(OPnetwork));
// 	network->ConnectSocket = INVALID_SOCKET;
// 	network->ConnectionType = networkType;
// 	return network;
// }
//
// i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPchar* port) {
// #ifdef OPIFEX_WINDOWS
// 	WSADATA wsaData;
// 	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
// 		fprintf(stderr, "WSAStartup failed.\n");
// 		return 1;
// 	}
// #endif
//
// 	struct addrinfo *result = NULL,
// 		*ptr = NULL,
// 		hints;
//
// 	OPbzero(&hints, sizeof(hints));
//
// 	hints.ai_family = AF_UNSPEC;
// 	hints.ai_socktype = SOCK_DGRAM;
// 	hints.ai_protocol = IPPROTO_UDP;
//
// 	// char strPort[6];
// 	// if (port == 0) {
// 	// 	OPmemcpy(strPort, DEFAULT_PORT, 6);
// 	// }
// 	// else {
// 	// 	sscanf(strPort, "%d", port);
// 	// }
//
// 	OPchar* addr;
// 	ui64 r = OPnetworkLookupAddress(address, &addr);
// 	OPlog("Address %d:%s", r, port);
// 	i32 iResult = getaddrinfo(addr, port, &hints, &result);
// 	OPlog("Get Addr Info: %d", iResult);
// 	OPfree(addr);
//
// 	if (iResult != 0) {
// 		OPlog("getaddrinfo failed: %d", iResult);
//
// 		if(iResult == EAI_AGAIN) {
// 			OPlog("EAI_AGAIN");
// 		}
// 		if(iResult == EAI_BADFLAGS) {
// 			OPlog("EAI_BADFLAGS");
// 		}
// 		if(iResult == EAI_FAIL) {
// 			OPlog("EAI_FAIL");
// 		}
// 		if(iResult == EAI_FAMILY) {
// 			OPlog("EAI_FAMILY");
// 		}
// 		if(iResult == EAI_MEMORY) {
// 			OPlog("EAI_MEMORY");
// 		}
// 		if(iResult == EAI_NONAME) {
// 			OPlog("EAI_NONAME");
// 		}
// 		if(iResult == EAI_OVERFLOW) {
// 			OPlog("EAI_OVERFLOW");
// 		}
// 		if(iResult == EAI_SYSTEM) {
// 			OPlog("EAI_SYSTEM");
// 		}
//
// 		WSA_CLEANUP();
// 		return 1;
// 	}
//
// 	ptr = result;
//
// 	network->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
//
// 	if (network->ConnectSocket == INVALID_SOCKET) {
// #ifdef OPIFEX_WINDOWS
// 		printf("Error at socket(): %ld\n", WSAGetLastError());
// #endif
// 		OPlog("INVALID SOCKET");
// 		WSA_CLEANUP();
// 		freeaddrinfo(result);
// 		return 1;
// 	}
//
// 	OPlog("Client connected to %s on port %s", address, port);
//
// 	return 0;
// }
//
// i32 OPnetworkServerStart(OPnetwork* network, OPchar* port) {
// #ifdef OPIFEX_WINDOWS
// 	WSADATA wsaData;
// 	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
// 		fprintf(stderr, "WSAStartup failed.\n");
// 		return 1;
// 	}
// #endif
//
// 	struct addrinfo hints;
// 	struct addrinfo *result = NULL, *ptr = NULL;
//
// 	OPbzero(&hints, sizeof (hints));
// 	hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_DGRAM;
// 	hints.ai_protocol = 0;//IPPROTO_UDP;
// 	hints.ai_flags = AI_PASSIVE;
//
// 	// char* strPort = (char*)OPalloc(6);
// 	// if (port == 0) {
// 	// 	OPmemcpy(strPort, DEFAULT_PORT, 6);
// 	// }
// 	// else {
// 	// 	sscanf(strPort, "%d", &port);
// 	// }
//
// 	// Resolve the local address and port to be used by the server
// 	OPlog("Get ADDR");
// 	OPchar* addr;
// 	ui64 r = OPnetworkLookupAddress("127.0.0.1", &addr);
// 	OPlog("Address %d:%s", r, port);
// 	i32 iResult = getaddrinfo(addr, port, &hints, &result);
// 	OPlog("Get Addr Info: %d", iResult);
// 	if (iResult != 0) {
// 		OPlog("GAI ERROR: %s", gai_strerror(iResult));
// 		OPlog("getaddrinfo failed: %d\n", iResult);
// 		WSA_CLEANUP();
// 		return 1;
// 	}
//
// 	OPlog("Connect Socket %d, %d, %d", result->ai_family, result->ai_socktype, result->ai_protocol);
// 	network->ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//
// 	if (network->ConnectSocket == INVALID_SOCKET) {
// #ifdef OPIFEX_WINDOWS
// 		printf("Error at socket(): %ld\n", WSAGetLastError());
// #endif
// 		OPlog("Invalid Socket");
// 		WSA_CLEANUP();
// 		freeaddrinfo(result);
// 		return 1;
// 	}
//
// 	OPlog("Bind Socket");
//
// 	iResult = bind(network->ConnectSocket, result->ai_addr, (i32)result->ai_addrlen);
// 	if (iResult == SOCKET_ERROR) {
// 		OPlog("Failed to bind socket");
// #ifdef OPIFEX_WINDOWS
// 		printf("bind failed with error: %d\n", WSAGetLastError());
// #endif
// 		WSA_CLEANUP();
// 		freeaddrinfo(result);
//
// #ifdef OPIFEX_WINDOWS
// 		closesocket(network->ConnectSocket);
// #else
// 		close(network->ConnectSocket);
// #endif
// 		return 1;
// 	}
// 	else{
// 		OPlog("Success - Listening");
// 		// if(listen(network->ConnectSocket, 64)){
// 		// 	const OPchar* errMsg = "No Error";
// 		// 	switch(errno){
// 		// 		case EACCES:
// 		// 			errMsg = "The current process has insufficient privileges.";
// 		// 			break;
// 		// 		case EBADF:
// 		// 			errMsg = "The argument socket is not a valid file descriptor.";
// 		// 			break;
// 		// 		case EDESTADDRREQ:
// 		// 			errMsg = "The socket is not bound to a local address and the protocol does not support listening on an unbound socket.";
// 		// 			break;
// 		// 		case EINVAL:
// 		// 			errMsg = "socket is already connected.";
// 		// 			break;
// 		// 		case ENOTSOCK:
// 		// 			errMsg = "The argument socket does not reference a socket.";
// 		// 			break;
// 		// 		case EOPNOTSUPP:
// 		// 			errMsg = "The socket is not of a type that supports the operation listen().";
// 		// 			break;
// 		// 	}
// 		// 	OPlog("Error on listen()\n\t- %s", errMsg);
// 		// }
// 	}
//
// 	freeaddrinfo(result);
//
// 	OPlog("Server started on port %s", port);
//
// 	return 0;
// }
//
// i32 OPnetworkReceive(OPnetwork* network, void* state, void(*receive)(void*, i32, OPchar*)) {
//
// 	OPchar recvbuf[DEFAULT_BUFLEN];
// 	i32 recvbuflen = DEFAULT_BUFLEN;
// 	i32 iResult = 0;
// 	sockaddr si_other;
// 	ui32 si_len = sizeof(si_other);
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
// 	// Receive until the peer shuts down the connection
// 	// do {
//
// 		// iResult = recv(network->ConnectSocket, recvbuf, recvbuflen, 0);
// 		// if(iResult > 0) {
// 		// 	OPlog("Bytes received: %d %d\n", iResult, *(int*)(recvbuf));
// 		// }
// 	// } while (true);
// 	//OPlog("Receiving Message...");
// 	//select()
// 	// if ((iResult = recvfrom(network->ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0, &si_other, &si_len)) == SOCKET_ERROR) {
// 	// 	return 1;
// 	// }
//
// 	//receive(state, iResult, recvbuf);
//
// 	return 0;
// }
//
// i32 OPnetworkSend(OPnetwork* network, i8* data, i32 size){
//
// 	OPchar recvbuf[DEFAULT_BUFLEN];
// 	i32 recvbuflen = DEFAULT_BUFLEN;
//
// 	struct addrinfo *result = NULL, *ptr = NULL, hints;
//
// 	OPbzero(&hints, sizeof (hints));
// 	hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_DGRAM;
// 	hints.ai_protocol = IPPROTO_UDP;
// 	hints.ai_flags = AI_PASSIVE;
//
// 	i32 iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
// 	if (iResult != 0) {
// 		printf("getaddrinfo for loop-back failed: %d\n", iResult);
// 		WSA_CLEANUP();
// 		return 1;
// 	}
//
// 	i32 slen = sizeof(*result->ai_addr);
//
// 	OPlog("Sending Message...");
// 	//send(network->ConnectSocket, recvbuf, iResult, 0);
// 	//return 1;
// 	if (sendto(network->ConnectSocket, data, size, 0, result->ai_addr, slen) == SOCKET_ERROR) {
// 		return 1;
// 	}
//
// 	return 0;
// }
//
// i32 OPnetworkShutdown(OPnetwork* network){
// 	i32 iResult = shutdown(network->ConnectSocket, SD_SEND);
// 	if (iResult == SOCKET_ERROR) {
// #ifdef OPIFEX_WINDOWS
// 		printf("shutdown failed: %d\n", WSAGetLastError());
// #endif
// 		WSA_CLEANUP();
// #ifdef OPIFEX_WINDOWS
// 		closesocket(network->ConnectSocket);
// #else
// 		close(network->ConnectSocket);
// #endif
// 		return 1;
// 	}
//
// #ifdef OPIFEX_WINDOWS
// 		closesocket(network->ConnectSocket);
// #else
// 		close(network->ConnectSocket);
// #endif
//
// 	WSA_CLEANUP();
//
// 	return 0;
// }
//
// void OPnetworkDestroy(OPnetwork* network) {
// 	OPfree(network);
// }
//
// #endif
