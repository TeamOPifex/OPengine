#include "./Communication/include/OPnetworkSocket.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

void OPnetworkSocket::Init(OPnetworkAddress address) {
    networkAddress = address;
    valid = false;

    ui32 networkSocketType = 
        networkAddress.networkSocketType == OPnetworkSocketType::UDP ? 
            SOCK_DGRAM : 
            SOCK_STREAM;

    ui32 networkFamily = 
        networkAddress.networkFamily == OPnetworkFamily::INET ? 
            SOCK_DGRAM : 
            SOCK_STREAM;
    
    connectedSocket = (i32)socket(
         networkFamily, 
         networkSocketType, 
         0);

 	if (connectedSocket == INVALID_SOCKET) {
		OPlogErr("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}



	struct addrinfo hints, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = networkSocketType;
	if ((rv = getaddrinfo(address.networkAddress, address.networkPort, &hints, &addrInfo)) != 0) {
		OPlogErr("getaddrinfo: %s\n", gai_strerror(rv));
		NETWORK_CLEANUP();
		return;
	}


    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr->sin_family = AF_UNSPEC;
    sockAddr->sin_addr.s_addr = inet_addr(address.networkAddress);
    sockAddr->sin_port = address.networkPortNum;
    
    
    if (connect(connectedSocket, (struct sockaddr *)&sockAddr, sizeof(struct sockaddr)) != 0) {
        OPlogErr("Error at connect()");
        CLOSESOCKET(connectedSocket);
        return;
    }



	// for(p = servinfo; p != NULL; p = p->ai_next) {
	// 	if ((connectedSocket = (i32)socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	// 		OPlogErr("Error at socket()");
	// 		continue;
	// 	}

    valid = true;
}

void OPnetworkSocket::Init(i32 socket, OPnetworkAddress address) {
    connectedSocket = socket;
    networkAddress = address;
    
    valid = true;
}

bool OPnetworkSocket::Send(void* data, ui32 size) {
    if (sendto(connectedSocket, data, size, 0, addrInfo->ai_addr, (i32)addrInfo->ai_addrlen) == -1) {
        OPlogErr("send error");
        return false;
    }
    return true;
}

bool OPnetworkSocket::Bind() {
	OPlogDebug("Bind Socket");
	i32 bindResult = bind(connectedSocket, addrInfo->ai_addr, (i32)addrInfo->ai_addrlen);
	freeaddrinfo(addrInfo);

	if (bindResult == SOCKET_ERROR) {
        return false;
    }


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

i32 OPnetworkSocket::Receive() {

    size_t len_to_receive = 0;
    ssize_t received_count = 0;
    size_t received_total = 0;

    do {
        // Is completely received?
        if (receiveBufferInd >= sizeof(receiveBuffer)) {
            // message_handler(&peer->receiving_buffer);
            OPlogInfo("Message from socket: %s", receiveBuffer);
            receiveBufferInd = 0;
        }

        // Count bytes to send.
        len_to_receive = sizeof(receiveBuffer) - receiveBufferInd;
        if (len_to_receive > MAX_SEND_SIZE)
            len_to_receive = MAX_SEND_SIZE;

        printf("Let's try to recv() %zd bytes... ", len_to_receive);

        received_count = recv(connectedSocket, (char *)&receiveBuffer + receiveBufferInd, len_to_receive, MSG_DONTWAIT);
        if (received_count < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                OPlogInfo("peer is not ready right now, try again later.\n");
            }
            else {
                OPlogErr("recv() from peer error");
                return -1;
            }
        }
        else if (received_count == 0) {
            OPlogInfo("recv() 0 bytes. Peer gracefully shutdown.\n");
            return -1;
        }
        else if (received_count > 0) {
            receiveBufferInd += received_count;
            received_total += received_count;
            printf("recv() %zd bytes\n", received_count);
        }

    } while (received_count > 0);
  
    printf("Total recv()'ed %zu bytes.\n", received_total);
    return 0;
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