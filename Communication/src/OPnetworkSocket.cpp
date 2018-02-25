#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

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

    OPlogInfo("socket was created");

    // OPlogInfo("getting network info for %s at port %s", address.networkAddressStr, address.networkPortStr);
	// struct addrinfo hints, *p;
	// int rv;
	// memset(&hints, 0, sizeof hints);
	// hints.ai_family = AF_UNSPEC;
	// hints.ai_socktype = networkSocketType;
	// if ((rv = getaddrinfo(address.networkAddressStr, address.networkPortStr, &hints, &addrInfo)) != 0) {
	// 	OPlogErr("getaddrinfo: %s\n", gai_strerror(rv));
	// 	NETWORK_CLEANUP();
	// 	return;
	// }

    if(networkSocketType == OPnetworkSocketType::STREAM){
        OPlogInfo("beginning connection");
        Connect();
    }

    valid = true;
}

bool OPnetworkSocket::Bind() {

    // lose the pesky "Address already in use" error message
    // int yes = 1;
    // if (setsockopt(connectedSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    //     OPlogErr("setsockopt");
    // }

    if(networkAddress.networkFamily == OPnetworkFamily::INET6) {
        struct sockaddr_in6 sin;
        bzero(&sin, sizeof(sin));
        sin.sin6_family = AF_INET6;
        //sin.sin6_addr = in6addr_any;
        inet_pton(AF_INET6, networkAddress.networkAddressStr, &sin.sin6_addr.s6_addr);
        sin.sin6_port = htons(networkAddress.networkPort);

        OPlogInfo("Binding to %s:%d", networkAddress.networkAddressStr, networkAddress.networkPort);

        OPlogDebug("Bind Socket");
        i32 bindResult = bind(connectedSocket, (struct sockaddr*)&sin, sizeof(sin));
        //networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen);

        if (bindResult == SOCKET_ERROR) {
            OPlogErr("socket failed to bind");
            return false;
        }

        OPlogInfo("socket has been bound");

        return true;

    } else {
        struct sockaddr_in sin;
        bzero(&sin, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htons(INADDR_ANY);
        sin.sin_port = htons(networkAddress.networkPort);

        OPlogDebug("Bind Socket");
        i32 bindResult = bind(connectedSocket, (struct sockaddr*)&sin, sizeof(sin));
        //networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen);

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
        i32 result = connect(connectedSocket, networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen); 
        if(result > 0) {
            return true;
        }

        OPlogErr("Failed to connect");
    }

    return false;
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
    (*networkSocket) = OPnetworkSocket();
    // OPbzero(&networkAddress.sockAddr);

    struct sockaddr address;
    int addrlen = sizeof(address);

    // OPbzero(&addr);
    
    i32 newSocket = accept(connectedSocket, &address, (socklen_t*)&addrlen);
    if(newSocket < 0) {
        OPlogErr("Failed to accept connection");
        return false;
    }

    OPlogInfo("New connection");// , socket fd is %d , ip is : %s , port : %d \n" , newSocket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
 
    struct sockaddr_storage* addrIn = (struct sockaddr_storage*)&address;
    OPnetworkAddress networkAddress = OPnetworkAddress(addrIn);
    networkSocket->Init(newSocket, networkAddress, networkSocketType == OPnetworkSocketType::STREAM ? OPnetworkProtocolType::TCP : OPnetworkProtocolType::UDP);

    return true;
}

#include "./Core/include/OPmemory.h"

i32 OPnetworkSocket::Send(void* data, ui32 size) {
    i32 byteSentCount;

    if(networkSocketType == OPnetworkSocketType::STREAM) {
        byteSentCount = send(connectedSocket, (i8*)data, size, 0);
    } else {

        if(networkAddress.networkFamily == OPnetworkFamily::INET6) {
	        struct sockaddr_in6 sin;
            bzero(&sin, sizeof(sin));
            sin.sin6_family = AF_INET6;
            sin.sin6_port = htons(networkAddress.networkPort);
            inet_pton(AF_INET6, networkAddress.networkAddressStr, &sin.sin6_addr.s6_addr);
            // OPmemcpy(networkAddress.networkAddressStr, sin.sin6_addr.s6_addr, 16);
            //inet_pton(AF_INET, "127.0.0.1", &sin.sin6_addr);

            //struct sockaddr* sendTo = (struct sockaddr*)&networkAddress.addr;
            //size_t s = networkAddress.networkFamily == OPnetworkFamily::INET ? sizeof(sockaddr_in) :sizeof(sockaddr_in6);

            byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, (struct sockaddr*)&sin, sizeof(sin));
        } else {
            struct sockaddr_in sin;
            bzero(&sin, sizeof(sin));
            sin.sin_family = AF_INET;
            inet_pton(AF_INET, networkAddress.networkAddressStr, &sin.sin_addr);
            sin.sin_port = htons(networkAddress.networkPort);

            //struct sockaddr* sendTo = (struct sockaddr*)&networkAddress.addr;
            //size_t s = networkAddress.networkFamily == OPnetworkFamily::INET ? sizeof(sockaddr_in) :sizeof(sockaddr_in6);

            byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, (struct sockaddr*)&sin, sizeof(sin));
        }
    }

    if(byteSentCount >= 0) {
        return byteSentCount;
    }

    OPlogErr("send error");
    return -1;
}

i32 OPnetworkSocket::Send(OPnetworkSocket* client, void* data, ui32 size) {
    i32 byteSentCount = 0;

    if(networkSocketType == OPnetworkSocketType::STREAM) {
	    byteSentCount = send(connectedSocket, (i8*)data, size, 0);
    } else {

        if(client->networkAddress.networkFamily == OPnetworkFamily::INET6) {

	        struct sockaddr_in6 sin;
            bzero(&sin, sizeof(sin));
            sin.sin6_family = AF_INET6;
            sin.sin6_port = htons(client->networkAddress.networkPort);
            //OPmemcpy(sin.sin6_addr.s6_addr, client->networkAddress.networkAddressStr, 16);
            inet_pton(AF_INET6, client->networkAddress.networkAddressStr, &sin.sin6_addr.s6_addr);

            OPlogInfo("Sending message (ipv6) to %s:%d", client->networkAddress.networkAddressStr, client->networkAddress.networkPort);
            
            byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, (struct sockaddr*)&sin, sizeof(sin));

        } else {
            struct sockaddr_in sin;
            bzero(&sin, sizeof(sin));
            sin.sin_family = AF_INET;
            inet_pton(AF_INET, client->networkAddress.networkAddressStr, &sin.sin_addr);
            sin.sin_port = htons(client->networkAddress.networkPort);

            OPlogInfo("Sending message (ipv4) to %s:%d", client->networkAddress.networkAddressStr, client->networkAddress.networkPort);
            
            byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, (struct sockaddr*)&sin, sizeof(sin));
        }

        
        // struct sockaddr* sendTo = (struct sockaddr*)&client->networkAddress.addr;
        // size_t s = client->networkAddress.networkFamily == OPnetworkFamily::INET ? sizeof(sockaddr_in) :sizeof(sockaddr_in6);

	    // byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, sendTo, s );
    }

	if (byteSentCount >= 0) {
		return byteSentCount;
	}
	OPlogErr("send to error: %d", errno);
	return -1;
}

i32 OPnetworkSocket::Receive(void* data, ui32 size) {
    i32 bytesRead = -1;

    #ifdef OPIFEX_WINDOWS
        bytesRead = recv(connectedSocket, (i8*)data, size, 0);
    #else
        bytesRead = recv(connectedSocket, (i8*)data, size, MSG_DONTWAIT);
    #endif

    if(bytesRead >= 0) {
        return bytesRead;
    }
    
    OPlogErr("receive error");
    return -1;
}

i32 OPnetworkSocket::ReceiveFrom(void* data, ui32 size, OPnetworkAddress* address) {

    socklen_t sockaddrSize = sizeof(struct sockaddr);
    struct sockaddr_storage clientSocketAddress;

    i32 bytesRead = -1;

    
    #ifdef OPIFEX_WINDOWS
        bytesRead = recvfrom(connectedSocket, (i8*)data, size, 0, (struct sockaddr *)&clientSocketAddress, &sockaddrSize);
    #else
        bytesRead = recvfrom(connectedSocket, data, size, 0, (struct sockaddr *)&clientSocketAddress, &sockaddrSize);
    


        i32 bytesSent = sendto(connectedSocket, data, bytesRead, 0, (struct sockaddr*)&clientSocketAddress, sockaddrSize);
        if(bytesSent <= 0) {
            OPlogErr("Still failed to send");
        } else {
            OPlogInfo("Send this time");
        }
    #endif

    
    address->Init(&clientSocketAddress);
    //addr, port, );

    if(bytesRead >= 0) {
        return bytesRead;
    }
    
    OPlogErr("receive from error");
    return -1;
}

i32 OPnetworkSocket::Update() {
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

    struct fd_set fds;
    FD_ZERO(&fds);
    FD_SET(connectedSocket, &fds);

    i32 selectResult = select(0, &fds, NULL, NULL, &timeout);
    
    if(selectResult == -1) {
		OPlogErr("Error with select()");
		return -1;
	}

    return selectResult;
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


void SetZero(fd_set* read, fd_set* write, fd_set* except) {
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(except);
}

void SetRead(fd_set* read, fd_set* write, fd_set* except) {
    
}