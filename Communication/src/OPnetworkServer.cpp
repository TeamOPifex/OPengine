#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Data/include/OPstring.h"
#include "./Communication/include/OPnetworkState.h"

void OPnetworkServer::Init(OPnetworkProtocolType::Enum protocol, ui32 port) {

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

    // // Gets the Loopback address on port
    OPnetworkAddress local = OPnetworkAddress(port);
    if(!local.valid) {
        OPlogErr("couldn't locate loopback address for port %d", port);
        return;
    }

    serverSocket = OPnetworkSocket(local, protocol);
    if(!serverSocket.valid) {
        return;
    }

    OPlogInfo("Socket created for loopback on port %d", port);

    serverSocket.Bind();

    for(ui32 i = 0; i < MAX_CLIENTS; i++) {
	    bzero(&clients[i], sizeof(clients[i]));
    }
    
    // Only does the full listen if TCP
    serverSocket.Listen();

    printf("listening\n");
}

void OPnetworkServer::Update() {
    selector.Zero();
    selector.SetRead(&serverSocket);

    if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
        for(ui32 i = 0; i < clientIndex; i++) {
            selector.SetRead(&clients[i]);
        }
    }

    i32 selectResult = selector.Select();
    if(selectResult > 0) {

        if(selector.IsReadSet(&serverSocket)) {

            if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
                // New Connection
                OPlogInfo("Handling new TCP connection");
                if(serverSocket.Accept(&clients[clientIndex])) {
                    if(ActiveNetworkState != NULL) {
                        ActiveNetworkState->Connected(&clients[clientIndex]);
                    }
                    clientIndex++;
                }
            } else {
                OPnetworkPacket packet;

                i32 bytes = serverSocket.ReceiveFrom(&packet, &clients[clientIndex]);

                // Look for an existing client socket
                OPnetworkSocket* existingClient = NULL;
                for(ui32 i = 0; i < clientIndex; i++) {
                    if(clients[i].Match(&clients[clientIndex])) {
                        existingClient = &clients[i];
                        break;
                    }
                }

                if(existingClient == NULL && OPstringEquals("CONNECT", packet.buffer)) {
                    OPlogInfo("New Client Found!");
                    existingClient = &clients[clientIndex];
                    existingClient->networkID = (OPNETWORK_ID++);
                    existingClient->verified = false;
                    existingClient->code = (ui8)(OPrandom() * (f32)sizeof(ui8));

                    // Send client their code to verify they are who they say there
                    OPnetworkPacket packetCode;
                    packetCode.I8(existingClient->code);
                    serverSocket.Send(existingClient, &packetCode);
					OPlogInfo("Server sent Code '%d' to Client to Verify", existingClient->code);

                    clientIndex++;
                } else if(existingClient != NULL) {
                    if(!existingClient->verified) {
                        if(existingClient->code == packet.buffer[0]) {
                            existingClient->verified = true;
                            OPlogInfo("Server Verified the client");
                            if(ActiveNetworkState != NULL) {
                                ActiveNetworkState->Connected(existingClient);
                            }
                        } else {
                            OPlogErr("Failed to verify client '%d'", existingClient->code);
                        }
                    } else if(ActiveNetworkState != NULL) {
                        ActiveNetworkState->Message(existingClient, &packet);
                    }                
                }

            }
        }

        if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
            for(ui32 i = 0; i < clientIndex; i++) {
                if(selector.IsReadSet(&clients[i])) {
                    OPnetworkPacket packet;
                    i32 bytes = clients[i].Receive(&packet);
                    if(bytes == 0) {
                        if(ActiveNetworkState != NULL) {
                            ActiveNetworkState->Disconnected(&clients[i]);
                        }

                        if(i != clientIndex - 1) {
                            clients[i] = clients[clientIndex - 1];
                            i--;
                        }
                        clientIndex--;
                    } else if(ActiveNetworkState != NULL) {
                        ActiveNetworkState->Message(&clients[i], &packet);
                    }
                }
            }
        }
    } else if(selectResult == -1) {
        OPlogErr("Failed to select");
    }
}

bool OPnetworkServer::Send(OPnetworkPacket* packet) {

    if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
        for(ui32 i = 0; i < clientIndex; i++) {
            clients[i].Send(packet);
        }
    } else {
        for(ui32 i = 0; i < clientIndex; i++) {
            serverSocket.Send(&clients[i], packet);
        }
    }
	return true;
}

void OPnetworkServer::Destroy() {

}