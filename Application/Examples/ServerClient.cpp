#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Communication/include/OPsimpleProtocol.h"


// Data for this Game State Example
typedef struct {
	i8 Mode;
	i8 HeldDown;
	OPnetworkProtocolSimple* Protocol;
} ServerClientExample;

ServerClientExample serverClientExampleExample;

void ExampleServerClientEnter(OPgameState* last) {
	serverClientExampleExample.Mode = 0;
	serverClientExampleExample.HeldDown = 0;
	serverClientExampleExample.Protocol = NULL;

}

void ClientReceive(OPprotocolSimpleMessage message) {
	OPlog("CLIENT RECEIVED MESSAGE");
	serverClientExampleExample.HeldDown = *message.Data;
}

void ServerReceive(OPprotocolSimpleMessage message) {
	OPlog("SERVER RECEIVED MESSAGE");
	serverClientExampleExample.HeldDown = *message.Data;
	OPnetworkProtocolSimpleSend(
		serverClientExampleExample.Protocol,
		NULL,
		(i8*)&serverClientExampleExample.HeldDown,
		sizeof(i8)
		);
}



OPint ExampleServerClientUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (serverClientExampleExample.Mode == 0 && OPkeyboardWasReleased(OPKEY_S)) {
		// Server Mode
		serverClientExampleExample.Mode = 1;
		serverClientExampleExample.Protocol = OPnetworkProtocolSimpleCreate(
			OPNETWORK_SERVER
		);
		OPnetworkServerStart(serverClientExampleExample.Protocol->Network, "1337");
		// OPnetworkProtocolSimpleReceive(
		// 	serverClientExampleExample.Protocol,
		// 	ServerReceive
		// );
	}
	if (serverClientExampleExample.Mode == 0 && OPkeyboardWasReleased(OPKEY_C)) {
		// Client Mode
		serverClientExampleExample.Mode = 2;
		serverClientExampleExample.Protocol = OPnetworkProtocolSimpleCreate(
			OPNETWORK_CLIENT
		);
		OPnetworkClientConnect(serverClientExampleExample.Protocol->Network, "127.0.0.1", "1337");
	}

	//if(serverClientExampleExample.Mode != 0 && OPmouseWasPressed(OPMOUSE_LBUTTON)) {

	//}
	if(serverClientExampleExample.Mode != 0 && OPmouseWasReleased(OPMOUSE_LBUTTON)) {
		serverClientExampleExample.HeldDown = 0;
		OPnetworkProtocolSimpleSend(
			serverClientExampleExample.Protocol,
			time,
			(i8*)&serverClientExampleExample.HeldDown,
			sizeof(i8)
		);
	}

	if(serverClientExampleExample.Mode == 1) {
		OPnetworkProtocolSimpleReceive(
			serverClientExampleExample.Protocol,
			ServerReceive
		);
	}
	
	if(serverClientExampleExample.Mode == 2) {
		OPnetworkProtocolSimpleReceive(
			serverClientExampleExample.Protocol,
			ClientReceive
		);
	}

	////////////////////////
	// Render
	////////////////////////
	if(serverClientExampleExample.HeldDown == 1) {
		// Someone is holding down the left mouse button
		OPrenderClear(1.0, 1.0, 1.0);
	} else if(serverClientExampleExample.Mode == 0) {
		// Server and Client not yet started
		OPrenderClear(0.0, 0.0, 0.0);
	}
	else if(serverClientExampleExample.Mode == 1) {
		// It's in server mode
		OPrenderClear(1.0, 0.0, 0.0);
	}
	else if(serverClientExampleExample.Mode == 2) {
		// It's in client mode
		OPrenderClear(0.0, 0.0, 1.0);
	}

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleServerClientExit(OPgameState* next) {

	return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_SERVER_CLIENT_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SERVER_CLIENT = {
	ExampleServerClientEnter,
	ExampleServerClientUpdate,
	ExampleServerClientExit
};
