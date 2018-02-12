#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkAddress.h"

#define OPNETWORKSOCKET_BUFFER_SIZE 1024
#define MAX_SEND_SIZE 1024

struct OPnetworkSocket {
    SOCKET connectedSocket;
    OPnetworkAddress networkAddress;
    bool valid = false;
    i8 receiveBuffer[OPNETWORKSOCKET_BUFFER_SIZE];
    i32 receiveBufferInd = 0;
    struct addrinfo* addrInfo;

    OPnetworkSocket() {}
    OPnetworkSocket(OPnetworkAddress address) {
        Init(address);
    }
    void Init(OPnetworkAddress address);
    void Init(i32 socket, OPnetworkAddress address);
	i32 Send(void* data, ui32 size);
	i32 Send(OPnetworkSocket* client, void* data, ui32 size);
    i32 Receive(void* data, ui32 size, sockaddr* sockAddr);
    i32 Update();
    bool Accept(OPnetworkSocket* networkSocket);
    bool Bind();
    void Destroy();
};