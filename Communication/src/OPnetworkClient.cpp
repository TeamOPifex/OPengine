#include "./Communication/include/OPnetworkClient.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPstring.h"
#include <string.h>

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
		network.LogError("Error getting address");
		NETWORK_CLEANUP();
		return;
	}

    socket.Init(networkAddress, protocolType);
    if(!socket.valid) {
		network.LogError("Error at socket()");
		NETWORK_CLEANUP();
 		return;
 	}
	
	const i8* connectMessage = "CONNECT";
	if (socket.Send((void*)connectMessage, strlen(connectMessage) + 1) < 0) {
		network.LogError("Error sending connect message");
	}

	// socket.Bind();

 	OPlogInfo("Client connected to %s on port %d", address, port);

 	return;
}

void OPnetworkClient::Update() {

	i8 buffer[1024];
    struct sockaddr_storage sockAddr;

	select.Zero();
	select.SetRead(&socket);
	// select.SetWrite(&socket);

	if (select.Select()) {
		//if (select.IsReadSet(&socket)) {
			int bytesReceived = socket.Receive(buffer, 1024);

			if (bytesReceived > 0) {
				if (OPstringEquals("CONNECT", buffer)) {
					return;
				}
				OPlogInfo("Message %s", buffer);

				if (receiveCallback != NULL) {
					receiveCallback(buffer, bytesReceived);
				}
			} else if (bytesReceived == 0) {
				// Server is disconnecting
			} else {
				OPlogErr("Error");
			}
		// } else {
		// 	OPlogInfo("Select returned but not for this read socket?");
		// }

		// if (select.IsWriteSet(&socket)) {
		// 	OPlogInfo("Write info");
		// }
	}// else {
	// 	OPlg(".");
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