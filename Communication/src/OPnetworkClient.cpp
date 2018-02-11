#include "./Communication/include/OPnetworkClient.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"

void OPnetworkClient::Init(OPnetworkProtocolType::Enum protocolType, const OPchar* address, ui32 port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
    networkAddress.Init(address, port, OPnetworkFamily::INET);
    if(!networkAddress.valid) {
		network.LogError("Error getting address");
		NETWORK_CLEANUP();
		return;
	}

    socket.Init(networkAddress);
    if(!socket.valid) {
		network.LogError("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}

	// FD_ZERO(&network.networkData.master);
	// FD_ZERO(&network.networkData.read_fds);
	// FD_SET(socket.connectedSocket, &network.networkData.master);
	// network.networkData.fdmax = socket.connectedSocket;

 	OPlogInfo("Client connected to %s on port %d", address, port);

 	return;
}

void OPnetworkClient::Update() {
    
	// i8 buf[BUFSIZE];
	// i32 nbytes;

	// struct timeval tv;
	// tv.tv_sec = 0;
	// tv.tv_usec = 1000;

	// i32 i;
	// i32 newfd;
	// struct sockaddr_storage remoteaddr;
	// socklen_t addrlen;

	// networkData.read_fds = networkData.master;
	// if (select(networkData.fdmax + 1, &networkData.read_fds, NULL, NULL, &tv) == -1) {
	// 	LogError("Error with select()");
	// 	return -1;
	// }

	// for (i = 0; i <= networkData.fdmax; i++) {
	// 	if (networkProtocol == OPnetworkProtocol::OPNETWORK_TCP) {
	// 		if (!FD_ISSET(i, &networkData.read_fds)) {
	// 			continue;
	// 		}
	// 	}

	// 	if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
	// 		if (nbytes == 0) {
	// 			OPlogErr("Connection Closed %d", i);
	// 		} else {
	// 			OPlogErr("Error receiving data on client");
	// 		}
	// 		CLOSESOCKET(i);
	// 		FD_CLR(i, &networkData.master); // remove from master set
	// 	}
	// 	else {
	// 		OPlog("Data Received From Client");
	// 		buf[nbytes] = NULL;
	// 		receive(state, i, nbytes, buf);
	// 	}
	// }
}

bool OPnetworkClient::Send(void* data, ui32 size) {
	i32 sent = socket.Send(data, size);
	if(sent == size) {
		OPlogInfo("Message sent");
		return true;
	} else if(sent == -1) {
		OPlogInfo("Message err, something happened");
		return false;
	}
}

void OPnetworkClient::Destroy() {
    
}