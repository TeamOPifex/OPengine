#include "./Communication/include/OPnetworkClient.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPstring.h"
#include "./Communication/include/OPnetworkState.h"

void OPnetworkClient::Init(OPnetworkProtocolType::Enum protocolType, const OPchar* address, ui32 port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

	// Get the address to use, if it's a computer name, url, it will DNS it
	OPnetworkAddress networkAddress;
    networkAddress.Init(address, port);
    if(!networkAddress.valid) {
		OPnetwork::LogError("Error getting address");
		NETWORK_CLEANUP();
		return;
	}

    clientSocket.Init(networkAddress, protocolType);
    if(!clientSocket.valid) {
		OPnetwork::LogError("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}
	
	// const i8* connectMessage = "CONNECT";
	// if (socket.Send((void*)connectMessage, strlen(connectMessage) + 1) < 0) {
	// 	network.LogError("Error sending connect message");
	// }

 	OPlogInfo("Client connected to %s on port %d", address, port);	
}

void OPnetworkClient::Update() {
	selector.Zero();
	selector.SetRead(&clientSocket);
	
	i32 selectResult = selector.Select();
	if(selectResult > 0) {
		if(selector.IsReadSet(&clientSocket)) {
			OPnetworkPacket packet;
			i32 bytes = clientSocket.Receive(&packet);
			if (bytes < 0) {
				OPlogErr("fail to receive.");
			}
			else {
				if(ActiveNetworkState != NULL) {
					ActiveNetworkState->Message(&clientSocket, &packet);
				}
			}
		}
	} else if(selectResult < 0) {
		OPlogErr("select error");
	}
}

bool OPnetworkClient::Send(OPnetworkPacket* packet) {
	clientSocket.Send(packet);
	return true;
}

void OPnetworkClient::Destroy() {
    
}