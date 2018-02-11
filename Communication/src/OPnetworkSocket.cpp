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


    OPlogInfo("getting network info for %s at port %s", address.networkAddress, address.networkPortStr);
	struct addrinfo hints, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = networkSocketType;
	if ((rv = getaddrinfo(address.networkAddress, address.networkPortStr, &hints, &addrInfo)) != 0) {
		OPlogErr("getaddrinfo: %s\n", gai_strerror(rv));
		NETWORK_CLEANUP();
		return;
	}

    OPlogInfo("beginning connection");



    // memset(&sockAddr, 0, sizeof(sockAddr));
    // sockAddr->sin_family = AF_UNSPEC;
    // sockAddr->sin_addr.s_addr = inet_addr(address.networkAddress);
    // sockAddr->sin_port = address.networkPort;
    
    
    // if (connect(connectedSocket, (struct sockaddr *)&sockAddr, sizeof(struct sockaddr)) != 0) {
    //     OPlogErr("Error at connect()");
    //     CLOSESOCKET(connectedSocket);
    //     return;
    // }



	// for(p = servinfo; p != NULL; p = p->ai_next) {
	// 	if ((connectedSocket = (i32)socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	// 		OPlogErr("Error at socket()");
	// 		continue;
	// 	}

    valid = true;
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
 
    // networkSocket->Init(newSocket, &address);

    return true;
}

void OPnetworkSocket::Init(i32 socket, OPnetworkAddress address) {
    connectedSocket = socket;
    networkAddress = address;
    
    valid = true;
}

i32 OPnetworkSocket::Send(void* data, ui32 size) {

	i32 byteSentCount = sendto(connectedSocket, data, size, 0, &networkAddress.sockAddr, sizeof(networkAddress.sockAddr));
    if(byteSentCount >= 0) {
        return byteSentCount;
    }
    OPlogErr("send error");
    return -1;
}

i32 OPnetworkSocket::Receive(void* data, ui32 size, sockaddr* sockAddr) {
    ui32 fromLength = sizeof(*sockAddr);

    int bytesRead = recv(connectedSocket, data, size, MSG_DONTWAIT);
    //int bytesRead = recvfrom(connectedSocket, data, size, 0, sockAddr, &fromLength);
    if(bytesRead >= 0) {
        return bytesRead;
    } else {
        // OPlogErr("Failed ro receive");
    }
}

bool OPnetworkSocket::Bind() {
	OPlogDebug("Bind Socket");
	i32 bindResult = bind(connectedSocket, &networkAddress.sockAddr, sizeof(networkAddress.sockAddr));
	// freeaddrinfo(addrInfo);

	if (bindResult == SOCKET_ERROR) {
        return false;
    }

    OPlogInfo("socket has been bound");

    return true;
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

// i32 OPnetworkSocket::Receive() {

//     size_t len_to_receive = 0;
//     ssize_t received_count = 0;
//     size_t received_total = 0;

//     do {
//         // Is completely received?
//         if (receiveBufferInd >= sizeof(receiveBuffer)) {
//             // message_handler(&peer->receiving_buffer);
//             OPlogInfo("Message from socket: %s", receiveBuffer);
//             receiveBufferInd = 0;
//         }

//         // Count bytes to send.
//         len_to_receive = sizeof(receiveBuffer) - receiveBufferInd;
//         if (len_to_receive > MAX_SEND_SIZE)
//             len_to_receive = MAX_SEND_SIZE;

//         printf("Let's try to recv() %zd bytes... ", len_to_receive);

//         received_count = recv(connectedSocket, (char *)&receiveBuffer + receiveBufferInd, len_to_receive, MSG_DONTWAIT);
//         if (received_count < 0) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                 OPlogInfo("peer is not ready right now, try again later.\n");
//             }
//             else {
//                 OPlogErr("recv() from peer error");
//                 return -1;
//             }
//         }
//         else if (received_count == 0) {
//             OPlogInfo("recv() 0 bytes. Peer gracefully shutdown.\n");
//             return -1;
//         }
//         else if (received_count > 0) {
//             receiveBufferInd += received_count;
//             received_total += received_count;
//             printf("recv() %zd bytes\n", received_count);
//         }

//     } while (received_count > 0);
  
//     printf("Total recv()'ed %zu bytes.\n", received_total);
//     return 0;
// }

void OPnetworkSocket::Destroy() {
    if(!valid) return;

    i32 iResult = shutdown(connectedSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        OPlogErr("shutdown failed");
        return;
    }

    CLOSESOCKET(connectedSocket);
}