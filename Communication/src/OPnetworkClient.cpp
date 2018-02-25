#include "./Communication/include/OPnetworkClient.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPstring.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void OPnetworkClient::Init(OPnetworkProtocolType::Enum protocolType, const OPchar* address, ui32 port) {

	// // On Windows the network has to be initialized
	// // Doesn't actually do anything on Unix based systems
	// if (OPnetwork::Initialize() != 0) {
    //     OPlogErr("Network could not be initialized");
	// 	return;
	// }

	// // Get the address to use, if it's a computer name, url, it will DNS it
	// OPnetworkAddress networkAddress;
    // networkAddress.Init(address, port);
    // if(!networkAddress.valid) {
	// 	network.LogError("Error getting address");
	// 	NETWORK_CLEANUP();
	// 	return;
	// }

    // socket.Init(networkAddress, protocolType);
    // if(!socket.valid) {
	// 	network.LogError("Error at socket()");
	// 	NETWORK_CLEANUP();
 	// 	return;
 	// }
	
	// const i8* connectMessage = "CONNECT";
	// if (socket.Send((void*)connectMessage, strlen(connectMessage) + 1) < 0) {
	// 	network.LogError("Error sending connect message");
	// }

	// // socket.Bind();

 	// OPlogInfo("Client connected to %s on port %d", address, port);

 	// return;




	bzero(&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	sin.sin_port = htons(port);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("fail to create socket.\n");
		exit(1);
	}
	
	addr_len = sizeof(cin);

	
}

void OPnetworkClient::Update() {

	// i8 buffer[1024];
    // struct sockaddr_storage sockAddr;

	// select.Zero();
	// select.SetRead(&socket);
	// // select.SetWrite(&socket);

	// if (select.Select()) {
	// 	if (select.IsReadSet(&socket)) {
	// 		int bytesReceived = socket.Receive(buffer, 1024);
	// 		OPlogInfo("Received: %d bytes", bytesReceived);

	// 		if (bytesReceived > 0) {
	// 			if (OPstringEquals("CONNECT", buffer)) {
	// 				return;
	// 			}
	// 			OPlogInfo("Message %s", buffer);

	// 			if (receiveCallback != NULL) {
	// 				receiveCallback(buffer, bytesReceived);
	// 			}
	// 		} else if (bytesReceived == 0) {
	// 			// Server is disconnecting
	// 		} else {
	// 			OPlogErr("Error");
	// 		}
	// 	 } else {
	// 		 OPlogInfo("Select returned but not for this read socket?");
	// 	 }

	// 	// if (select.IsWriteSet(&socket)) {
	// 	// 	OPlogInfo("Write info");
	// 	// }
	// }// else {
	// // 	OPlg(".");
	// // }



	fd_set fds;
	struct timeval timeout;	

    timeout.tv_usec = 100;
    timeout.tv_sec = 0;

    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);

    maxfdp = sockfd + 1;


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
			if (FD_ISSET(sockfd, &fds)) {

				i32 bytes = recvfrom(sockfd, buf, MAX_LINE, 0, (struct sockaddr *)&cin, &addr_len);

				if (bytes < 0) {
					perror("fail to receive.\n");
					exit(1);
				}
				else {
					printf("receive from server: %s.\n", buf);
					receiveCallback(buf, bytes);
					// if (close(sockfd) < 0) {
					//     perror("fail to close.\n");
					//     exit(1);
					// }
					//return 0;
				}
			}
			break;
	}
}

bool OPnetworkClient::Send(void* data, ui32 size) {
	// i32 sent = socket.Send(data, size);
	// if(sent == size) {
	// 	OPlogInfo("Message sent");
	// 	return true;
	// } else if(sent == -1) {
	// 	OPlogInfo("Message err, something happened");
	// 	return false;
	// }

    sendto(sockfd, data, size, 0, (struct sockaddr*)&sin, sizeof(sin));

	return true;
}

void OPnetworkClient::Destroy() {
    
}