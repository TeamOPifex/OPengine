#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkAddress.h"

#define OPNETWORKSOCKET_BUFFER_SIZE 1024
#define MAX_SEND_SIZE 1024

struct OPnetworkSocket {
    i32 connectedSocket;
    OPnetworkAddress networkAddress;
    bool valid;
    i8 receiveBuffer[OPNETWORKSOCKET_BUFFER_SIZE];
    i32 receiveBufferInd = 0;
    struct addrinfo* addrInfo;
    struct sockaddr_in* sockAddr;

    OPnetworkSocket() {}
    OPnetworkSocket(OPnetworkAddress address) {
        Init(address);
    }
    void Init(OPnetworkAddress address);
    void Init(i32 socket, OPnetworkAddress address);
    bool Send(void* data, ui32 size);
    i32 Update();
    i32 Receive();
    bool Bind();
    void Destroy();
};