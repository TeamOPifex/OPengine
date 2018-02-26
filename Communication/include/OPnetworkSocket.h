#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkAddress.h"

#define OPNETWORKSOCKET_BUFFER_SIZE 1024
#define MAX_SEND_SIZE 1024

struct OPnetworkSocket {
    struct sockaddr_storage sockAddr;
    socklen_t sockAddrLen;
    SOCKET connectedSocket;
    OPnetworkAddress networkAddress;
    OPnetworkSocketType::Enum networkSocketType;
    bool valid = false;

    OPnetworkSocket() {}
    OPnetworkSocket(OPnetworkAddress address, OPnetworkProtocolType::Enum protocol) {
        Init(address, protocol);
    }
    void Init(OPnetworkAddress address, OPnetworkProtocolType::Enum protocol);
    void Init(i32 socket, OPnetworkAddress address, OPnetworkProtocolType::Enum protocol);

	i32 Send(void* data, ui32 size);
	i32 Send(OPnetworkSocket* client, void* data, ui32 size);
    i32 Receive(void* data, ui32 size);
    i32 ReceiveFrom(void* data, ui32 size, OPnetworkSocket* networkSocket);
    bool Match(OPnetworkSocket* socket);
    i32 Update();

    bool Accept(OPnetworkSocket* networkSocket);
    bool Bind();
    bool Listen();    
    bool Connect();

    void Destroy();

    static void Select(fd_set* read, fd_set* write, fd_set* except);
};