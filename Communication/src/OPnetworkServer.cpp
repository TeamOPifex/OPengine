#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

void OPnetworkServer::Init(OPnetworkProtocolType::Enum protocolType, ui32 port) {
	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("Network could not be initialized");
		return;
	}

    //OPnetworkAddress local = OPnetworkAddress("127.0.0.1", port, OPnetworkFamily::INET);
    OPnetworkAddress local = OPnetworkAddress(port, OPnetworkFamily::INET);
    if(!local.valid) {
        OPlogErr("couldn't locate loopback address for port %s", port);
        return;
    }

    OPlogInfo("Loopback Address created for port %d", port);

    serverSocket = OPnetworkSocket(local);
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

}

void OPnetworkServer::UpdateTCP() {
    // if (select(networkData.fdmax + 1, &networkData.read_fds, NULL, NULL, &tv) == -1) {
	// 	OPlogErr("Error with select()");
	// 	return -1;
	// }

    // for (i = 0; i <= networkData.fdmax; i++) {
    //     if (!FD_ISSET(i, &networkData.read_fds)) {
    //         continue;
    //     }

    //     if (i == connectedSocket) {
    //         // New connection
    //         addrlen = sizeof remoteaddr;
    //         struct sockaddr* tmp = (struct sockaddr *)&remoteaddr;

    //         newfd = (i32)accept(connectedSocket,
    //             tmp,
    //             &addrlen
    //             );

    //         if (newfd == -1) {
    //             OPlogErr("Failed to Accept Client %s", tmp->sa_data);
    //         } else {

    //             OPlogInfo("CLIENT CONNECTED");
    //             FD_SET(newfd, &networkData.master);
    //             if (newfd > networkData.fdmax) {
    //                 networkData.fdmax = newfd;
    //             }
    //             connected(state, i);

    //             // OPlog("New connection from %s on socket %d",
    //             // 	inet_ntop(remoteaddr.ss_family,
    //             // 	get_in_addr((struct sockaddr*)&remoteaddr),
    //             // 	remoteIP, INET6_ADDRSTRLEN),
    //             // 	newfd
    //             // );
    //         }
    //     }
    //     else {
    //         // Data from client
    //         if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
    //             if (nbytes == 0) {
    //                 OPlog("Connection Closed %d", i);
    //             }
    //             else {
    //                 OPlog("Error receiving data");
    //             }
    //             CLOSESOCKET(i);
    //             FD_CLR(i, &networkData.master); // remove from master set
    //             disconnected(state, i);
    //         }
    //         else {
    //             OPlog("Data Received From Client");
    //             buf[nbytes] = NULL;
    //             receive(state, i, nbytes, buf);
    //         }
    //     }
    // }
}

int BuildFDSets(OPnetworkServer* networkServer, OPnetworkSocket serverSocket, fd_set *read_fds, fd_set *write_fds, fd_set *except_fds)
{
    int i;

    FD_ZERO(read_fds);
    FD_SET(STDIN_FILENO, read_fds);
    FD_SET(serverSocket.connectedSocket, read_fds);
    for (i = 0; i < MAX_CLIENTS; ++i)
        if (networkServer->clients[i].connectedSocket != INVALID_SOCKET)
            FD_SET(networkServer->clients[i].connectedSocket, read_fds);

    FD_ZERO(write_fds);
    for (i = 0; i < MAX_CLIENTS; ++i)
        if (networkServer->clients[i].connectedSocket != INVALID_SOCKET && networkServer->clients[i].receiveBufferInd > 0)
            FD_SET(networkServer->clients[i].connectedSocket, write_fds);

    FD_ZERO(except_fds);
    FD_SET(STDIN_FILENO, except_fds);
    FD_SET(serverSocket.connectedSocket, except_fds);
    for (i = 0; i < MAX_CLIENTS; ++i)
        if (networkServer->clients[i].connectedSocket != INVALID_SOCKET)
            FD_SET(networkServer->clients[i].connectedSocket, except_fds);

    return 0;
}  

int OPnetworkServer::HandleNewConnection()
{
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = sizeof(client_addr);

    int new_client_sock = accept(serverSocket.connectedSocket, (struct sockaddr *)&client_addr, &client_len);
    if (new_client_sock < 0) {
        OPlogErr("accept()");
        return -1;
    }
    
    char client_ipv4_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);

    OPnetworkAddress clientAddress = OPnetworkAddress(client_ipv4_str, client_addr.sin_port, OPnetworkFamily::INET);
    
    OPlogInfo("Incoming connection from %s:%d.\n", client_ipv4_str, client_addr.sin_port);
    
    int i;
    for (i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connectedSocket == INVALID_SOCKET) {
            clients[i].Init(new_client_sock, clientAddress);

            // clients[i].connectedSocket = new_client_sock;
            // connection_list[i].addres = client_addr;
            // connection_list[i].current_sending_byte   = -1;
            // connection_list[i].current_receiving_byte = 0;
            return 0;
        }
    }
    
    printf("There is too much connections. Close new connection %s:%d.\n", client_ipv4_str, client_addr.sin_port);
    close(new_client_sock);
    return -1;
}

void OPnetworkServer::UpdateUDP() {

	i8 buf[BUFSIZE];
	i32 nbytes;

	i32 i;
	i32 newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;

    BuildFDSets(this, serverSocket, &read_fds, &write_fds, &except_fds);

    i32 highestSocket = serverSocket.connectedSocket;
    for (i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connectedSocket > highestSocket)
            highestSocket = clients[i].connectedSocket;
    }

    i32 activity = serverSocket.Update();

    if(activity == -1) {
        OPlogErr("select() error");
    } else if(activity == 0) {
        OPlogErr("Shouldn't ever get here...");
    } else {

        /* All set fds should be checked. */
        // if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        //     if (handle_read_from_stdin() != 0)
        //         shutdown_properly(EXIT_FAILURE);
        // }

        if (FD_ISSET(serverSocket.connectedSocket, &read_fds)) {
            HandleNewConnection();
        }
        
        // if (FD_ISSET(STDIN_FILENO, &except_fds)) {
        //   printf("except_fds for stdin.\n");
        //   shutdown_properly(EXIT_FAILURE);
        // }

        if (FD_ISSET(serverSocket.connectedSocket, &except_fds)) {
            OPlogErr("Exception listen socket fd.\n");
            // shutdown_properly(EXIT_FAILURE);
        }
        
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if(clients[i].connectedSocket == INVALID_SOCKET) {
                continue;
            }

            if (FD_ISSET(clients[i].connectedSocket, &read_fds)) {
                // if(clients[i].Receive() != 0) {
                //     clients[i].Destroy();
                // }
            }
    
            if (FD_ISSET(clients[i].connectedSocket, &write_fds)) {
                // if (send_to_peer(&clients[i]) != 0) {
                //     clients[i].Destroy();
                //     continue;
                // }
            }

            if (FD_ISSET(clients[i].connectedSocket, &except_fds)) {
                OPlogErr("Exception client fd.\n");
                clients[i].Destroy();
                continue;
            }
        }
    }
    

	// for (i = 0; i <= networkData.fdmax; i++) {
	// 	if (!FD_ISSET(i, &networkData.read_fds)) {
	// 		continue;
	// 	}

	// 	// This socket is ready

    //     socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
    //     int s;
    //     struct sockaddr_storage peer_addr;

    //     // Data from client
    //     if ((nbytes = recvfrom(i, buf, sizeof buf, 0,
    //         (struct sockaddr *) &peer_addr, &peer_addr_len)) <= 0) {
    //         if (nbytes == 0) {
    //             OPlog("Connection Closed %d", i);
    //         }
    //         else {
    //             OPlog("Error receiving data");
    //         }
    //         CLOSESOCKET(i);
    //         FD_CLR(i, &networkData.master); // remove from master set
    //     }
    //     else {

    //         OPlog("Data Received From Client");
    //         char host[NI_MAXHOST], port[NI_MAXSERV];

    //         s = getnameinfo((struct sockaddr *) &peer_addr,
    //             peer_addr_len, host, NI_MAXHOST,
    //             port, NI_MAXSERV, NI_NUMERICSERV);

    //         if (s == 0) {
    //             OPlog("Received %zd bytes from %s:%s\n", nbytes, host, port);
    //         } else {
    //             OPlogErr("getnameinfo: %s\n", gai_strerror(s));
    //             OPlog("Received %zd bytes from %s:%s\n", nbytes, host, port);
    //         }

    //         i32 found = 0;
    //         i32 i = 0;
    //         for (; i < networkData.clientIndex; i++) {
    //             if (OPstringEquals(host, networkData.peer_host[i]) &&
    //                 OPstringEquals(port, networkData.peer_port[i])) {
    //                 OPlog("Found existing");
    //                 found = 1;
    //                 break;
    //             }
    //         }

    //         if (!found) {
    //             OPlog("NEW CLIENT");
    //             networkData.peer_addr[networkData.clientIndex] = peer_addr;
    //             networkData.peer_host[networkData.clientIndex] = OPstringCopy(host);
    //             networkData.peer_port[networkData.clientIndex] = OPstringCopy(port);
    //             connected(state, networkData.clientIndex);
    //             networkData.clientIndex++;
    //         }

    //         OPlog("Calling Receive");
    //         buf[nbytes] = NULL;
    //         receive(state, i, nbytes, buf);
    //     }		
	// }
}

fd_set* OPnetworkServerFill(OPnetworkServer* networkServer, fd_set& outSet) {
    FD_ZERO(&outSet);
    // for(ui32 i = 0; i < networkServer->clientIndex; i++) {
    //     FD_SET(networkServer->clients[i].connectedSocket, &outSet);
    // }
    return &outSet;
}

void OPnetworkServerFillSet(OPnetworkServer* networkServer, OPnetworkSocket** outSockets, ui32* index, fd_set& inSet ) {
    *index = 0;

    if(FD_ISSET(networkServer->serverSocket.connectedSocket, &inSet)) {
        OPlogInfo("Set server socket as out read");
        outSockets[(*index)++] = &networkServer->serverSocket;
    }
    // for(ui32 i = 0; i < networkServer->clientIndex; i++) {
    //     if(FD_ISSET(networkServer->clients[i].connectedSocket, &inSet)) {
    //         outSockets[(*index)++] = &networkServer->clients[i];
    //     }
    // }
}

int OPnetworkServer::Select() {
    fd_set read, write, except;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

    fd_set* readPtr = OPnetworkServerFill(this, read);
        FD_SET(serverSocket.connectedSocket, &read);
    fd_set* writePtr = OPnetworkServerFill(this, write);
    fd_set* exceptPtr = OPnetworkServerFill(this, except);

    i32 highest = serverSocket.connectedSocket;
    for(ui32 i = 0; i < clientIndex; i++) {
        if(highest < clients[i].connectedSocket) {
            highest = clients[i].connectedSocket;
        }
    }

    int toRet = select(highest + 1, readPtr, writePtr, exceptPtr, &timeout);

    if(toRet > 0) {
        // process sockets
        // OPlogInfo("toRet: %d", toRet);
        OPnetworkServerFillSet(this, clientsToRead, &clientsToReadIndex, read);
        // OPnetworkServerFillSet(this, clientsToWrite, &clientsToWriteIndex, write);
        // OPnetworkServerFillSet(this, clientsToExcept, &clientsToExceptIndex, except);
    }

    return toRet;
}

void OPnetworkServer::Update() {

    i8 buffer[1024];
    

    if(Select()) {
        // OPlogInfo("Select finished");
        
        i8 rcvbuffer[1024];

        sockaddr sockAddr;
        int bytesReceived = serverSocket.Receive(buffer, 1024, &sockAddr);
        if(bytesReceived > 0) {
            OPlogInfo("Message %s", buffer);
        }

        sockaddr_in* sockAddrIn = (sockaddr_in*)&sockAddr;
        // OPlogInfo("Received packet from %s:%d", inet_ntoa(sockAddrIn->sin_addr), ntohs(sockAddrIn->sin_port));
        

        // Look for existing client
        bool found = false;
        for(ui32 i = 0; i < clientIndex; i++) {
            if(clients[i].networkAddress.SockAddrIn()->sin_addr.s_addr == sockAddrIn->sin_addr.s_addr &&
                clients[i].networkAddress.SockAddrIn()->sin_port == sockAddrIn->sin_port) {
                // found the client
                found = true;
                break;
            }
        }

        if(!found) {
            // add the client
            OPlogInfo("A new client has been found");
            OPnetworkAddress address = OPnetworkAddress(sockAddrIn);
            clients[clientIndex].Init(address);
            clientIndex++;
        }
        

        // if (recvfrom(s, rcvbuffer, 1024, 0, (struct sockaddr*)&sa_in, sizeof(sa_in)) >= 0)
        // {
        // for(ui32 i = 0; i < clientsToReadIndex; i++) {
        //     if(clientsToRead[i]->connectedSocket == serverSocket.connectedSocket) {
        //         // this is a new connection
        //         OPlogInfo("A new connection was made to the server");
        //         // Create new connection
        //         if(serverSocket.Accept(&clients[clientIndex])) {
        //             clientIndex++;
        //         }
                
        //     } else {
        //         // normal message, process it
        //         int bytesReceived = clientsToRead[i]->Receive(buffer, 1024);
        //         if(bytesReceived > 0) {
        //             OPlogInfo("Message %s", buffer);
        //         }

        //     }
        // }
    }

}


bool OPnetworkServer::Send(void* data, ui32 size) {
	int j;

	if (serverSocket.networkAddress.networkSocketType == OPnetworkSocketType::STREAM) {
        // TODO: (garrett) TCP Network Send
		// for (j = 0; j <= networkData.fdmax; j++) {
		// 	// send to everyone!
		// 	if (FD_ISSET(j, &networkData.master)) {
		// 		// except the listener and ourselves
		// 		if (j != connectedSocket) {
		// 			if (send(j, data, size, 0) == -1) {
		// 				OPlogErr("Send error");
		// 			}
		// 		}
		// 	}
		// }
	}
	else {
		socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
		for (i32 i = 0; i < clientIndex; i++) {
            clients[i].Send(data, size);
		}
	}
    
	return true;
}

void OPnetworkServer::Destroy() {

}