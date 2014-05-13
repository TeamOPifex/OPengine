#include "./include/Network.h"
#include "./Core/include/Log.h"
#include "./Core/include/DynamicMemory.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#include <stdlib.h> 

ui64 OPnetworkLookupAddress(const OPchar *kpAddress, OPchar** resolved)
{
	ui64 a;

	if ((a = inet_addr(kpAddress)) == INADDR_NONE)
	{
		hostent* pHE = gethostbyname(kpAddress);

		if (pHE == 0)
		{
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
				memcpy(*resolved, ad, len);
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

i32 OPnetworkClientConnect(OPnetwork* network, OPchar* address, OPint port) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	char* strPort = (char*)OPalloc(6);
	if (port == 0) {
		memcpy(strPort, DEFAULT_PORT, 6);
	}
	else {
		itoa(port, strPort, 10);
	}

	OPchar* addr;
	ui64 r = OPnetworkLookupAddress(address, &addr);
	i32 iResult = getaddrinfo(addr, strPort, &hints, &result);
	OPfree(strPort);
	OPfree(addr);

	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ptr = result;

	network->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (network->ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	return 0;
}

i32 OPnetworkServerStart(OPnetwork* network, OPint port) {

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
		return 1;
	}


	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	char* strPort = (char*)OPalloc(6);
	if (port == 0) {
		memcpy(strPort, DEFAULT_PORT, 6);
	}
	else {

		itoa(port, strPort, 10);
	}

	// Resolve the local address and port to be used by the server
	i32 iResult = getaddrinfo(NULL, strPort, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	network->ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (network->ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(network->ConnectSocket, result->ai_addr, (i32)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(network->ConnectSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	return 0;
}

OPnetwork* OPnetworkAcceptClient(OPnetwork* network) {
	if (network->ConnectionType == OPNETWORK_CLIENT) return NULL;

	OPnetwork* client = (OPnetwork*)OPalloc(sizeof(OPnetwork));
	client->ConnectSocket = INVALID_SOCKET;
	client->ConnectionType = OPNETWORK_CLIENT;

	OPlog("Waiting for client...");
	client->ConnectSocket = accept(network->ConnectSocket, NULL, NULL);
	if (client->ConnectSocket == INVALID_SOCKET) {
		OPlog("accept failed: %d\n", WSAGetLastError());
		closesocket(client->ConnectSocket);
		free(client);
		WSACleanup();
		return NULL;
	}

	return client;
}

i32 OPnetworkReceive(OPnetwork* network, void(*receive)(i32, OPchar*)) {

	OPchar recvbuf[DEFAULT_BUFLEN];
	i32 recvbuflen = DEFAULT_BUFLEN;
	i32 iResult;
	sockaddr si_other;
	i32 si_len = sizeof(si_other);

	while (1) {
		fflush(stdout);
		memset(recvbuf, '\0', DEFAULT_BUFLEN);

		if ((iResult = recvfrom(network->ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0, &si_other, &si_len)) == SOCKET_ERROR) {
			return 1;
		}

		receive(iResult, recvbuf);
	}

	return 0;
}

i32 OPnetworkSend(OPnetwork* network, void* data, i32 size){

	OPchar recvbuf[DEFAULT_BUFLEN];
	i32 recvbuflen = DEFAULT_BUFLEN;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	i32 iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	i32 slen = sizeof(*result->ai_addr);

	if (sendto(network->ConnectSocket, (OPchar*)data, size, 0, result->ai_addr, slen) == SOCKET_ERROR) {
		return 1;
	}

	return 0;
}

i32 OPnetworkShutdown(OPnetwork* network){
	i32 iResult = shutdown(network->ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(network->ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(network->ConnectSocket);
	WSACleanup();

	return 0;
}

void OPnetworkDestroy(OPnetwork* network) {
	OPfree(network);
}