#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"


void OPnetworkServer::Init(OPnetworkProtocolType::Enum protocol, ui32 port) {

    // protocolType = protocol;

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

    // // Gets the Loopback address on port
    // OPnetworkAddress local = OPnetworkAddress(port);
    // if(!local.valid) {
    //     OPlogErr("couldn't locate loopback address for port %d", port);
    //     return;
    // }

    // OPlogInfo("Loopback Address created for port %d", port);
    // serverSocket = OPnetworkSocket(local, protocolType);
    // if(!serverSocket.valid) {
    //     return;
    // }

	if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
		perror("fail to create socket.\n");
		exit(1);
	}

    // OPlogInfo("Socket created for loopback on port %d", port);

    // if(!serverSocket.Bind()) {
	// 	OPlogErr("Failed to bind socket");
    //     serverSocket.Destroy();
    //     return;
    // }

    for(ui32 i = 0; i < MAX_CLIENTS; i++) {
	    bzero(&clients[i], sizeof(clients[i]));
    }

	bzero(&sin, sizeof(sin));
	sin.sin6_family = AF_INET6;
	//sin.sin6_addr.s_addr = htons(INADDR_ANY);
	sin.sin6_port = htons(port);

	n = bind(sockfd, (struct sockaddr*)&sin, sizeof(sin));
	if (n < 0) {
		perror("fail to bind.\n");
		exit(1);
	}

	// OPlogInfo("Server started on port %d", port);

    // // Only does the full listen if TCP
    // serverSocket.Listen();


    printf("listening\n");

}

void OPnetworkServer::Update() {

    // i8 buffer[1024];
    
    // select.Zero();
    // select.SetRead(&serverSocket);
    // for(ui32 i = 0; i < clientIndex; i++) {
    //     select.SetRead(&clients[i]);
    // }

    // // Only listen on other client sockets if TCP
    // if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
    //     // listen for other connected sockets (only when TCP)
    //     for(ui32 i = 0; i < clientIndex; i++) {
    //         select.SetRead(&clients[i]);
    //     }
    // }

    // if(select.Select()) {

    //     // Handle New Connection
    //     if(select.IsReadSet(&serverSocket)) {
    //         if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
    //             // Handle TCP Connection
    //             // new connection
    //             if(!serverSocket.Accept(&clients[clientIndex])) {
    //                 OPlogErr("Failed to accept new connection");
    //             }
    //         } else {
                
    //             // Handle UDP Message
    //             OPnetworkAddress addressFrom;
    //             int bytesReceived = serverSocket.ReceiveFrom(buffer, 1024, &addressFrom);
    //             if(bytesReceived > 0) {
    //                 OPlogInfo("Message From %s: %s", addressFrom.networkAddressStr, buffer);

    //                 // Look for existing client
    //                 OPnetworkSocket* client = NULL;
    //                 for(ui32 i = 0; i < clientIndex; i++) {

    //                     OPlogInfo("CLIENT: %s\n", clients[i].networkAddress.networkAddressStr);
    //                     // if(clients[i].networkAddress.Match(&addr)) {
    //                     //     client = &clients[i];
    //                     //     break;
    //                     // }
    //                     if(clients[i].networkAddress.networkPort == addressFrom.networkPort) {
    //                         client = &clients[i];
    //                         break;
    //                     }
    //                 }

    //                 const OPchar* connectMessage = "CONNECT";
    //                 if(client == NULL) {
    //                     // client wasn't found
    //                     // if the message was CONNECT
    //                     // it's their first time, add them
    //                     if (OPstringEquals(connectMessage, buffer)) {
    //                         // Send messages back to this address
    //                         clients[clientIndex].Init(addressFrom, serverSocket.networkSocketType == OPnetworkSocketType::STREAM ? OPnetworkProtocolType::TCP : OPnetworkProtocolType::UDP);
    //                         client = &clients[clientIndex];
    //                         clientIndex++;

    //                         if (clientConnectedCallback != NULL && client != NULL) {
    //                             clientConnectedCallback(client);
    //                         }
                            
    //                         if(!serverSocket.Send(client, (void*)connectMessage, strlen(connectMessage) + 1)) {
    //                             OPlogErr("Failed to send CONNECT msg");
    //                         }
    //                     }
    //                     else {
    //                         // bad message
    //                         OPlogErr("Client shouldn't be connecting?");
    //                     }
    //                 }
    //                 else {
    //                     // Found the client, hit the callback
    //                     if (receiveCallback != NULL && client != NULL) {
    //                         receiveCallback(client, buffer, bytesReceived);
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     // On TCP more than just the bound socket can have incoming data
    //     if(serverSocket.networkSocketType == OPnetworkSocketType::STREAM) {
    //         for(ui32 i = 0; i < clientIndex; i++) {
    //             if(select.IsReadSet(&clients[i])) {
    //                 // This socket sent something, read it
    //                 i32 bytesReceived = clients[i].Receive(buffer, 1024);
    //                 if(bytesReceived == 0) {
    //                     // client disconnected
    //                     if(clientDisconnectedCallback != NULL) {
    //                         clientDisconnectedCallback(&clients[i]);
    //                     }
                        
    //                     // push back
    //                     if(i < clientIndex - 1) {
    //                         clients[i] = clients[clientIndex - 1];
    //                     }
    //                     clientIndex--;

    //                 } else if (bytesReceived >= 0 && receiveCallback != NULL) {
    //                     OPlogInfo("Message Received: %s", buffer);
    //                     receiveCallback(&clients[i], buffer, bytesReceived);
    //                 }
    //             }
    //         }
    //     }
        
    // }

	fd_set fds;
	struct timeval timeout;


    timeout.tv_usec = 100;
    timeout.tv_sec = 0;

    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);

    maxfdp = sockfd + 1;
    switch(select(maxfdp, &fds, NULL, NULL, &timeout)) {
        case -1: 
            perror("error on select");
            exit(-1);	
            break;

        case 0:
            // printf("timeout.\n");
            break;

        default:
	        bzero(&clients[clientIndex], sizeof(clients[clientIndex]));
            addr_len = sizeof(clients[clientIndex]);
            ui32 bytes = recvfrom(sockfd, buf, MAX_LINE, 0, (struct sockaddr*)&clients[clientIndex], &addr_len);

            // Look for an existing client socket
            struct sockaddr_storage* existingClient = NULL;
            for(ui32 i = 0; i < clientIndex; i++) {
                if(clients[i].ss_family == AF_INET6 && clients[clientIndex].ss_family == AF_INET6) {
                    sockaddr_in6* ipv6existing = (sockaddr_in6*)&clients[i];
                    sockaddr_in6* ipv6new = (sockaddr_in6*)&clients[clientIndex];


                    int r = memcmp(ipv6existing->sin6_addr.s6_addr, ipv6new->sin6_addr.s6_addr, sizeof(ipv6existing->sin6_addr.s6_addr));

                    if(ipv6existing->sin6_port == ipv6new->sin6_port && 
                        r == 0) {
                           existingClient = &clients[i];
                            OPlogInfo("Existing IPv6 Client Found!");
                           break;
                    }

                } else if(clients[i].ss_family == AF_INET && clients[clientIndex].ss_family == AF_INET) {
                    sockaddr_in* ipv4existing = (sockaddr_in*)&clients[i];
                    sockaddr_in* ipv4new = (sockaddr_in*)&clients[clientIndex];

                    if(ipv4existing->sin_port == ipv4new->sin_port && 
                        ipv4existing->sin_addr.s_addr == ipv4new->sin_addr.s_addr) {
                           existingClient = &clients[i];
                            OPlogInfo("Existing IPv4 Client Found!");
                           break;
                    }

                }
            }

            if(existingClient == NULL) {
                OPlogInfo("New Client Found!");
                clientIndex++;
            }


            receiveCallback(NULL, buf, bytes);

    }
}


bool OPnetworkServer::Send(void* data, ui32 size) {
	// int j;

    // for (i32 i = 0; i < clientIndex; i++) {
    //     OPnetworkSocket* client = &clients[i];
    //     serverSocket.Send(client, data, size);
    //     OPlogInfo("Sent: %d bytes", size);
    //     // if (client->Send(data, size) < 0) {
    //     //     OPnetwork::LogError("Send Err");
    //     // }
    // }

    // printf("Message: %s\n", d);

    for(ui32 i = 0; i < clientIndex; i++) {
        sendto(sockfd, data, size, 0, (struct sockaddr*)&clients[i], addr_len);
    }
    
	return true;
}

void OPnetworkServer::Destroy() {

}