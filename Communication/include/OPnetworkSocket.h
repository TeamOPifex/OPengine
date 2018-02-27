#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPacket.h"

#define OPNETWORKSOCKET_BUFFER_SIZE 1024
#define MAX_SEND_SIZE 1024

extern ui32 OPNETWORK_ID;

struct OPnetworkSocket {
    struct sockaddr_storage sockAddr;
    socklen_t sockAddrLen;
    SOCKET connectedSocket;
    OPnetworkAddress networkAddress;
    OPnetworkSocketType::Enum networkSocketType;
    bool valid = false;
    ui32 networkID;
    ui8 code = 0;
    bool verified = false;
    i64 verifyTimer = 0;
	OPnetworkPacket networkPacket;
    void* userData = NULL;

	OPnetworkSocket() {
        
	}

    OPnetworkSocket(OPnetworkAddress address, OPnetworkProtocolType::Enum protocol) {
        Init(address, protocol);
    }
    void Init(OPnetworkAddress address, OPnetworkProtocolType::Enum protocol);
    void Init(i32 socket, OPnetworkAddress address, OPnetworkProtocolType::Enum protocol);

	i32 Send(OPnetworkPacket* packet);
	i32 Send(OPnetworkSocket* client, OPnetworkPacket* packet);
    i32 Receive();
    i32 ReceiveFrom(OPnetworkSocket* networkSocket);
    bool Match(OPnetworkSocket* socket);

    bool Accept(OPnetworkSocket* networkSocket);
    bool Bind();
    bool Listen();    
    bool Connect();
    bool Verify(ui8 code);
    bool GenCode();

	bool CanBindIP4OnIP6();

    void Destroy();
};