#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"

ui32 OPNETWORK_ID = 0;

i32 OPnetworkSocketTypeToCode(OPnetworkSocketType::Enum networkSocketType) {
    switch(networkSocketType) {
        case OPnetworkSocketType::STREAM:
            return SOCK_STREAM;
        case OPnetworkSocketType::DGRAM:
            return SOCK_DGRAM;
        case OPnetworkSocketType::RAW:
            OPlogErr("RAW SOCKET TYPE IS NOT SUPPORTED");
            return SOCK_RAW;
        case OPnetworkSocketType::DGRAM_SEQ:
            OPlogErr("DGRAM SEQ SOCKET TYPE IS NOT SUPPORTED");
            return SOCK_SEQPACKET;
        default:
            return SOCK_DGRAM;
    }
    return SOCK_DGRAM;
}

void OPnetworkSocket::Init(i32 socket, OPnetworkAddress address, OPnetworkProtocolType::Enum protocol) {
    connectedSocket = socket;
    networkAddress = address;    
	networkSocketType = protocol == OPnetworkProtocolType::TCP ? OPnetworkSocketType::STREAM : OPnetworkSocketType::DGRAM;
    
    valid = true;
}

void OPnetworkSocket::Init(OPnetworkAddress address, OPnetworkProtocolType::Enum protocol) {
    networkAddress = address;
    valid = false;
    networkID = (OPNETWORK_ID++);

    networkSocketType = protocol == OPnetworkProtocolType::TCP ? OPnetworkSocketType::STREAM : OPnetworkSocketType::DGRAM;

    ui32 networkFamily = OPnetworkFamilyTypeToCode(networkAddress.networkFamily);
    ui32 networkSocketTypeCode = OPnetworkSocketTypeToCode(networkSocketType);
    ui32 networkProtocolType = 0;
    
    OPlogInfo("creating socket %d, %d, %d", networkFamily, networkSocketType, networkProtocolType);

    connectedSocket = (i32)socket(
         networkFamily, 
         networkSocketTypeCode, 
         networkProtocolType);

 	if (connectedSocket == INVALID_SOCKET) {
		OPlogErr("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}

    bzero(&sockAddr, sizeof(sockAddr));
    if(networkAddress.networkFamily == OPnetworkFamily::INET6) {
        struct sockaddr_in6* sin = (struct sockaddr_in6*)&sockAddr;
        bzero(sin, sizeof(struct sockaddr_in6));
        sin->sin6_family = AF_INET6;
        inet_pton(AF_INET6, networkAddress.networkAddressStr, &sin->sin6_addr);
        sin->sin6_port = htons(networkAddress.networkPort);
        sockAddrLen = sizeof(struct sockaddr_in6);
    } else {
        struct sockaddr_in* sin = (struct sockaddr_in*)&sockAddr;
        bzero(sin, sizeof(struct sockaddr_in));
        sin->sin_family = AF_INET;
        inet_pton(AF_INET, networkAddress.networkAddressStr, &sin->sin_addr);
        sin->sin_port = htons(networkAddress.networkPort);
        sockAddrLen = sizeof(struct sockaddr_in);
    }
    

    OPlogInfo("socket was created");

    valid = true;
}

bool OPnetworkSocket::Bind() {

    // lose the pesky "Address already in use" error message
    int yes = 1;
    if (setsockopt(connectedSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        OPlogErr("setsockopt");
    }

    if(networkAddress.networkFamily == OPnetworkFamily::INET6) {
        struct sockaddr_in6* sin = (struct sockaddr_in6*)&sockAddr;

        OPlogInfo("Binding ipv6 to %s:%d", networkAddress.networkAddressStr, networkAddress.networkPort);

        i32 bindResult = bind(connectedSocket, (struct sockaddr*)sin, sockAddrLen);

        if (bindResult == SOCKET_ERROR) {
            OPlogErr("socket failed to bind");
            return false;
        }

        OPlogInfo("socket has been bound");

        return true;

    } else {
        struct sockaddr_in* sin = (struct sockaddr_in*)&sockAddr;

        OPlogInfo("Binding ipv4 to %s:%d", networkAddress.networkAddressStr, networkAddress.networkPort);

        i32 bindResult = bind(connectedSocket, (struct sockaddr*)sin, sockAddrLen);

        if (bindResult == SOCKET_ERROR) {
            OPlogErr("socket failed to bind");
            return false;
        }

        OPlogInfo("socket has been bound");

        return true;
    }
}

bool OPnetworkSocket::Connect() {
    if(networkSocketType == OPnetworkSocketType::STREAM) {    
        i32 result = connect(connectedSocket, (struct sockaddr*)&sockAddr, sockAddrLen); 
        if(result == 0) {
            OPlogInfo("Connected to TCP server");
            return true;
        }

        OPlogErr("Failed to connect");
        return false;
    }

    return true;
}

bool OPnetworkSocket::Listen() {
    if(networkSocketType == OPnetworkSocketType::STREAM) {
        i32 result = listen(connectedSocket, 10);
        if(result < 0) {
            OPlogErr("Failed to begin listen()");
            return false;
        } 
        return true;
    }
    return false;
}

bool OPnetworkSocket::Accept(OPnetworkSocket* networkSocket) {

    OPbzero(&networkSocket->sockAddr, sizeof(networkSocket->sockAddr));    
    networkSocket->sockAddrLen = sizeof(networkSocket->sockAddr);
    
    i32 newSocket = accept(connectedSocket, (struct sockaddr*)&networkSocket->sockAddr, &networkSocket->sockAddrLen);
    if(newSocket < 0) {
        OPlogErr("Failed to accept connection");
        return false;
    }

    networkSocket->connectedSocket = newSocket;
    networkSocket->networkAddress = OPnetworkAddress(&networkSocket->sockAddr);

    OPlogInfo("New connection %s:%d", networkSocket->networkAddress.networkAddressStr, networkSocket->networkAddress.networkPort);
 
    return true;
}

#include "./Core/include/OPmemory.h"

i32 OPnetworkSocket::Send(OPnetworkPacket* packet) {
    i32 bytesSent = 0;
    i32 totalSent = 0;    
    while(totalSent < packet->size) {
        bytesSent = sendto(connectedSocket, &packet->buffer[totalSent], packet->size - totalSent, 0, (struct sockaddr*)&sockAddr, sockAddrLen);
        if(bytesSent < 0) {
            OPlogErr("Send error");
            return bytesSent;
        }

        totalSent += bytesSent;
    }

    return bytesSent;
}

i32 OPnetworkSocket::Send(OPnetworkSocket* client, OPnetworkPacket* packet) {
    i32 bytesSent = 0;
    i32 totalSent = 0;
    while(totalSent < packet->size) {

        // if(networkSocketType == OPnetworkSocketType::STREAM) {
        //     bytesSent = send(connectedSocket, &packet->buffer[totalSent], packet->size - totalSent, 0);
        // } else {
            bytesSent = sendto(connectedSocket, &packet->buffer[totalSent], packet->size - totalSent, 0, (struct sockaddr*)&client->sockAddr, client->sockAddrLen);
        //}

        if(bytesSent < 0) {
            OPlogErr("Send to error");
            return bytesSent;
        }

        totalSent += bytesSent;
    }
    return bytesSent;
}

i32 OPnetworkSocket::Receive(OPnetworkPacket* packet) {
    i32 bytesRead = -1;

    #ifdef OPIFEX_WINDOWS
        bytesRead = recv(connectedSocket, (i8*)packet->buffer, MAX_PACKET_SIZE, 0);
    #else
        bytesRead = recv(connectedSocket, (i8*)packet->buffer, MAX_PACKET_SIZE, MSG_DONTWAIT);
    #endif

    if(bytesRead >= 0) {
        packet->size = bytesRead;
        return bytesRead;
    }
    
    OPlogErr("receive error");
    return -1;
}

i32 OPnetworkSocket::ReceiveFrom(OPnetworkPacket* packet, OPnetworkSocket* networkSocket) {

    bzero(&networkSocket->sockAddr, sizeof(networkSocket->sockAddr));
    networkSocket->sockAddrLen = sizeof(networkSocket->sockAddr);
    ui32 bytesRead = recvfrom(connectedSocket, packet->buffer, MAX_PACKET_SIZE, 0, (struct sockaddr*)&networkSocket->sockAddr, &networkSocket->sockAddrLen);
    if(bytesRead >= 0) {
        packet->size = bytesRead;
        return bytesRead;
    }
    
    OPlogErr("receive from error");
    return -1;
}

void OPnetworkSocket::Destroy() {
    if(!valid) return;

    i32 iResult = shutdown(connectedSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        OPlogErr("shutdown failed");
        return;
    }

    CLOSESOCKET(connectedSocket);
}

bool OPnetworkSocket::Match(OPnetworkSocket* socket) {
    if(sockAddr.ss_family == AF_INET6 && socket->sockAddr.ss_family == AF_INET6) {
        sockaddr_in6* ipv6existing = (sockaddr_in6*)&sockAddr;
        sockaddr_in6* ipv6new = (sockaddr_in6*)&socket->sockAddr;

        int r = memcmp(ipv6existing->sin6_addr.s6_addr, ipv6new->sin6_addr.s6_addr, sizeof(ipv6existing->sin6_addr.s6_addr));

        if(ipv6existing->sin6_port == ipv6new->sin6_port && 
            r == 0) {
                // OPlogInfo("Existing IPv6 Client Found!");
                return true;
        }

    } else if(sockAddr.ss_family == AF_INET && socket->sockAddr.ss_family == AF_INET) {
        sockaddr_in* ipv4existing = (sockaddr_in*)&sockAddr;
        sockaddr_in* ipv4new = (sockaddr_in*)&socket->sockAddr;

        if(ipv4existing->sin_port == ipv4new->sin_port && 
            ipv4existing->sin_addr.s_addr == ipv4new->sin_addr.s_addr) {
                // OPlogInfo("Existing IPv4 Client Found!");
                return true;
        }
    }

    return false;
}


bool OPnetworkSocket::Verify(ui8 c) {
    if(c == code) {
        verified = true;
        networkID = (OPNETWORK_ID++);
        return true;
    }
    return false;
}

bool OPnetworkSocket::GenCode() {
    verified = false;
    code = (ui8)(OPrandom() * (f32)sizeof(ui8));
    verifyTimer = 1000; // 1 second to verify
}