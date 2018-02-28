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

	if(clientSocket.Connect()) {
 		OPlogInfo("Client connected to %s on port %d", address, port);	

		clientSocket.verified = false;
		OPnetworkPacket packetConnect;
		packetConnect.Str("CONNECT");
		clientSocket.Send(&packetConnect);
		OPlogInfo("Client Sent CONNECT");
	}
}

void OPnetworkClient::Update() {
	selector.Zero();
	selector.SetRead(&clientSocket);
	
	i32 selectResult = selector.Select();
	if(selectResult > 0) {
		if(selector.IsReadSet(&clientSocket)) {

			i32 bytes = clientSocket.Receive();
			clientSocket.networkPacket.buffer.Rewind();

			if (bytes < 0) {
				OPlogErr("fail to receive.");
			}
			else {
				if(!clientSocket.verified) {
					clientSocket.verified = true;

					OPnetworkPacket packetVerify;
					packetVerify.I8(clientSocket.networkPacket.I8());
					clientSocket.Send(&packetVerify);
					
					OPlogInfo("Client Verified");
					
					if(ActiveNetworkState != NULL) {
						ActiveNetworkState->Connected(&clientSocket);
					}

				} else {
					if(ActiveNetworkState != NULL) {
						
						if(clientSocket.networkPacket.buffer.PeekI8() == 0) {
							// ping/pong packet, send back a 0
							clientSocket.networkPacket.buffer.I8();
							OPnetworkPacket packet;
							packet.I8(0);
							// Send(&packet);
							OPlogInfo("Sending ping/pong back");
						} else {
							ActiveNetworkState->Message(&clientSocket, &clientSocket.networkPacket);
						}
						clientSocket.networkPacket.buffer.FastForward();
					}
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