#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>


#ifdef OPIFEX_UNIX
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>

	#define NETWORK_CLEANUP(){ }
	#define SD_SEND SHUT_WR
	#define CLOSESOCKET(socket) close(socket);
	#define SOCKET i32
#endif

#ifdef OPIFEX_WINDOWS
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <Windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	#pragma comment(lib, "Ws2_32.lib")
	
	#define NETWORK_CLEANUP() { WSACleanup(); }
	#define CLOSESOCKET(socket) closesocket(socket);
#endif
 
#define BUFSIZE 2048
#define SOCKET_ERROR -1
#ifndef INVALID_SOCKET
	#define INVALID_SOCKET -1
#endif