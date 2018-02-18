#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

void OPnetworkServer::Init(OPnetworkProtocolType::Enum protocol, ui32 port) {

    protocolType = protocol;

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

    // Gets the Loopback address on port
    OPnetworkAddress local = OPnetworkAddress(port);
    if(!local.valid) {
        OPlogErr("couldn't locate loopback address for port %d", port);
        return;
    }

    OPlogInfo("Loopback Address created for port %d", port);
    serverSocket = OPnetworkSocket(local, protocolType);
    if(!serverSocket.valid) {
        return;
    }

    OPlogInfo("Socket created for loopback on port %d", port);

    if(!serverSocket.Bind()) {
		OPlogErr("Failed to bind socket");
        serverSocket.Destroy();
        return;
    }

	OPlogInfo("Server started on port %d", port);

    serverSocket.Listen();
}

void OPnetworkServer::Update() {

    i8 buffer[1024];
    // sockaddr sockAddr;
    struct sockaddr_storage addr;
    
    select.Zero();
    select.SetRead(&serverSocket);
    for(ui32 i = 0; i < clientIndex; i++) {
        select.SetRead(&clients[i]);
    }

    if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
        // listen for other connected sockets (only when TCP)
        for(ui32 i = 0; i < clientIndex; i++) {
            select.SetRead(&clients[i]);
        }
    }

    if(select.Select()) {

        // Handle New Connection
        if(select.IsReadSet(&serverSocket)) {
            if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
                // Handle TCP Connection
                // new connection
                if(!serverSocket.Accept(&clients[clientIndex])) {
                    OPlogErr("Failed to accept new connection");
                }
            } else {
                // Handle UDP Connection
                OPnetworkAddress addressFrom;
                int bytesReceived = serverSocket.ReceiveFrom(buffer, 1024, &addressFrom);
                if(bytesReceived > 0) {
                    OPlogInfo("Message From %s: %s", addressFrom.networkAddressStr, buffer);

                    // Look for existing client
                    OPnetworkSocket* client = NULL;
                    for(ui32 i = 0; i < clientIndex; i++) {

                        OPlogInfo("CLIENT: %s\n", clients[i].networkAddress.networkAddressStr);
                        // if(clients[i].networkAddress.Match(&addr)) {
                        //     client = &clients[i];
                        //     break;
                        // }
                        if(clients[i].networkAddress.networkPort == addressFrom.networkPort) {
                            client = &clients[i];
                            break;
                        }
                    }

                    const OPchar* connectMessage = "CONNECT";
                    if(client == NULL) {
                        // client wasn't found
                        // if the message was CONNECT
                        // it's their first time, add them
                        if (OPstringEquals(connectMessage, buffer)) {
                            // Send messages back to this address
                            clients[clientIndex].Init(addressFrom, serverSocket.networkSocketType == OPnetworkSocketType::STREAM ? OPnetworkProtocolType::TCP : OPnetworkProtocolType::UDP);
                            client = &clients[clientIndex];
                            clientIndex++;

                            if (clientConnectedCallback != NULL && client != NULL) {
                                clientConnectedCallback(client);
                            }
                            
                            if(!serverSocket.Send(client, (void*)connectMessage, strlen(connectMessage) + 1)) {
                                OPlogErr("Failed to send CONNECT msg");
                            }
                        }
                        else {
                            // bad message
                            OPlogErr("Client shouldn't be connecting?");
                        }
                    }
                    else {
                        // Found the client, hit the callback
                        if (receiveCallback != NULL && client != NULL) {
                            receiveCallback(client, buffer, bytesReceived);
                        }
                    }
                }
            }
        }

        if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
            for(ui32 i = 0; i < clientIndex; i++) {
                if(select.IsReadSet(&clients[i])) {
                    // This socket sent something, read it
                    i32 bytesReceived = clients[i].Receive(buffer, 1024);
                    if(bytesReceived == 0) {
                        // client disconnected
                        if(clientDisconnectedCallback != NULL) {
                            clientDisconnectedCallback(&clients[i]);
                        }
                        
                        // push back
                        if(i < clientIndex - 1) {
                            clients[i] = clients[clientIndex - 1];
                        }
                        clientIndex--;

                    } else if (bytesReceived >= 0 && receiveCallback != NULL) {
                        OPlogInfo("Message Received: %s", buffer);
                        receiveCallback(&clients[i], buffer, bytesReceived);
                    }
                }
            }
        }
        
    }

}


bool OPnetworkServer::Send(void* data, ui32 size) {
	int j;

    for (i32 i = 0; i < clientIndex; i++) {
        OPnetworkSocket* client = &clients[i];
        //serverSocket.Send(client, data, size);
        if (client->Send(data, size) < 0) {
            OPnetwork::LogError("Send Err");
        }
    }
    
	return true;
}

void OPnetworkServer::Destroy() {

}