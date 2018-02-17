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

void OPnetworkSocket::Init(OPnetworkAddress address) {
    networkAddress = address;
    valid = false;

    ui32 networkFamily = OPnetworkFamilyTypeToCode(networkAddress.networkFamily);
    ui32 networkSocketType = OPnetworkSocketTypeToCode(networkAddress.networkSocketType);
    ui32 networkProtocolType = 0;
    
    OPlogInfo("creating socket %d, %d, %d", networkFamily, networkSocketType, networkProtocolType);

    connectedSocket = (i32)socket(
         networkFamily, 
         networkSocketType, 
         networkProtocolType);

 	if (connectedSocket == INVALID_SOCKET) {
		OPlogErr("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}

    OPlogInfo("socket was created");

    OPlogInfo("getting network info for %s at port %s", address.networkAddressStr, address.networkPortStr);
	struct addrinfo hints, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = networkSocketType;
	if ((rv = getaddrinfo(address.networkAddressStr, address.networkPortStr, &hints, &addrInfo)) != 0) {
		OPlogErr("getaddrinfo: %s\n", gai_strerror(rv));
		NETWORK_CLEANUP();
		return;
	}

    OPlogInfo("beginning connection");

    if(networkAddress.networkSocketType == OPnetworkSocketType::STREAM){
        Connect();
    }

    valid = true;
}

bool OPnetworkSocket::Bind() {

    // lose the pesky "Address already in use" error message
    int yes = 1;
    if (setsockopt(connectedSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        OPlogErr("setsockopt");
    } 


	OPlogDebug("Bind Socket");
	i32 bindResult = bind(connectedSocket, networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen);

	if (bindResult == SOCKET_ERROR) {
        OPlogErr("socket failed to bind");
        return false;
    }

    OPlogInfo("socket has been bound");

    return true;
}

bool OPnetworkSocket::Connect() {
    if(networkAddress.networkSocketType == OPnetworkSocketType::STREAM) {    
        i32 result = connect(connectedSocket, networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen); 
        if(result > 0) {
            return true;
        }

        OPlogErr("Failed to connect");
    }

    return false;
}

bool OPnetworkSocket::Listen() {
    if(networkAddress.networkSocketType == OPnetworkSocketType::STREAM) {
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
    
    i32 newSocket = accept(connectedSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if(newSocket < 0) {
        OPlogErr("Failed to accept connection");
        return false;
    }

    OPlogInfo("New connection");// , socket fd is %d , ip is : %s , port : %d \n" , newSocket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
 
    OPnetworkAddress networkAddress = OPnetworkAddress(&address, networkAddress.networkSocketType == OPnetworkSocketType::STREAM ? OPnetworkProtocolType::TCP : OPnetworkProtocolType::UDP );
    networkSocket->Init(newSocket, networkAddress);

    return true;
}

void OPnetworkSocket::Init(i32 socket, OPnetworkAddress address) {
    connectedSocket = socket;
    networkAddress = address;
    
    valid = true;
}

i32 OPnetworkSocket::Send(void* data, ui32 size) {
    i32 byteSentCount;

    if(networkAddress.networkSocketType == OPnetworkSocketType::STREAM) {
        byteSentCount = send(connectedSocket, (i8*)data, size, 0);
    } else {
        byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, networkAddress.addressInfo->ai_addr, networkAddress.addressInfo->ai_addrlen);
    }

    if(byteSentCount >= 0) {
        return byteSentCount;
    }

    OPlogErr("send error");
    return -1;
}

i32 OPnetworkSocket::Send(OPnetworkSocket* client, void* data, ui32 size) {
    i32 byteSentCount = 0;

    struct sockaddr_in* inAddr = (struct sockaddr_in*)&client->networkAddress.addr;

    if(networkAddress.networkSocketType == OPnetworkSocketType::STREAM) {
	    byteSentCount = send(connectedSocket, (i8*)data, size, 0);
    } else {
	    byteSentCount = sendto(connectedSocket, (i8*)data, size, 0, &client->networkAddress.addr, sizeof(*inAddr));
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
    struct sockaddr_in clientSocketAddress;

    i32 bytesRead = -1;
    #ifdef OPIFEX_WINDOWS
        bytesRead = recvfrom(connectedSocket, (i8*)data, size, 0, (struct sockaddr *)&clientSocketAddress, &sockaddrSize);
    #else
        bytesRead = recvfrom(connectedSocket, data, size, 0, (struct sockaddr *)&clientSocketAddress, &sockaddrSize);
    #endif

    
    address->Init((struct sockaddr *)&clientSocketAddress, networkAddress.networkSocketType == OPnetworkSocketType::STREAM ? OPnetworkProtocolType::TCP : OPnetworkProtocolType::UDP);
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