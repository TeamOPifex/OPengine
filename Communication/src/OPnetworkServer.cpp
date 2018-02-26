#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
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

    i32 selectResult = selector.Select();
    if(selectResult > 0) {

        if(selector.IsReadSet(&serverSocket)) {
            OPnetworkPacket packet;

            ui32 bytes = serverSocket.ReceiveFrom(&packet, &clients[clientIndex]);

            // Look for an existing client socket
            OPnetworkSocket* existingClient = NULL;
            for(ui32 i = 0; i < clientIndex; i++) {
                if(clients[i].Match(&clients[clientIndex])) {
                    existingClient = &clients[i];
                    break;
                }
            }

            if(existingClient == NULL) {
                OPlogInfo("New Client Found!");
                existingClient = &clients[clientIndex];
                clientIndex++;
            }

            if(ActiveNetworkState != NULL) {
                ActiveNetworkState->OnMessage(existingClient, &packet);
            }

            if(receiveCallback != NULL) {
                receiveCallback(existingClient, packet.buffer, packet.size);
            }
        }
    } else if(selectResult == -1) {
        OPlogErr("Failed to select");
    }
}

bool OPnetworkServer::Send(OPnetworkPacket* packet) {
    for(ui32 i = 0; i < clientIndex; i++) {
        serverSocket.Send(&clients[i], packet);
    }
	return true;
}

void OPnetworkServer::Destroy() {

}