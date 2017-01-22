#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Communication/include/OPsimpleProtocol.h"




void ClientReceive(OPprotocolSimpleMessage message);
void ServerReceive(OPprotocolSimpleMessage message);

// Data for this Game State Example
class ServerClientExample : public OPgameState {
public:
	i8 Mode;
	i8 HeldDown;
	OPnetworkProtocolSimple* Protocol;


	void Init(OPgameState* last) {
		Mode = 0;
		HeldDown = 0;
		Protocol = NULL;
	}


	OPint Update(OPtimer* time) {

		////////////////////////
		// Update
		////////////////////////

		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::S)) {
			// Server Mode
			Mode = 1;
			Protocol = OPnetworkProtocolSimpleCreate(
				OPNETWORK_SERVER
			);
			OPnetworkServerStart(Protocol->Network, "1337");
			// OPnetworkProtocolSimpleReceive(
			// 	Protocol,
			// 	ServerReceive
			// );
		}
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::C)) {
			// Client Mode
			Mode = 2;
			Protocol = OPnetworkProtocolSimpleCreate(
				OPNETWORK_CLIENT
			);
			OPnetworkClientConnect(Protocol->Network, "127.0.0.1", "1337");
		}

		//if(Mode != 0 && OPmouseWasPressed(OPmouseButton::LBUTTON)) {

		//}
		if (Mode != 0 && OPMOUSE.WasReleased(OPmouseButton::LBUTTON)) {
			HeldDown = 0;
			OPnetworkProtocolSimpleSend(
				Protocol,
				time,
				(i8*)&HeldDown,
				sizeof(i8)
			);
		}

		if (Mode == 1) {
			OPnetworkProtocolSimpleReceive(
				Protocol,
				ServerReceive
			);
		}

		if (Mode == 2) {
			OPnetworkProtocolSimpleReceive(
				Protocol,
				ClientReceive
			);
		}

		////////////////////////
		// Render
		////////////////////////
		if (HeldDown == 1) {
			// Someone is holding down the left mouse button
			OPrenderClear(1.0, 1.0, 1.0);
		}
		else if (Mode == 0) {
			// Server and Client not yet started
			OPrenderClear(0.0, 0.0, 0.0);
		}
		else if (Mode == 1) {
			// It's in server mode
			OPrenderClear(1.0, 0.0, 0.0);
		}
		else if (Mode == 2) {
			// It's in client mode
			OPrenderClear(0.0, 0.0, 1.0);
		}

		// Swaps the back buffer
		OPrenderPresent();

		// Tells the engine to continue running
		// Returning true will tell the engine to terminate
		return false;

	}
	void Render(OPfloat delta) {

	}
	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {

		return 0;
	}
};

ServerClientExample _GS_EXAMPLE_SERVER_CLIENT;

void ClientReceive(OPprotocolSimpleMessage message) {
	OPlog("CLIENT RECEIVED MESSAGE");
	_GS_EXAMPLE_SERVER_CLIENT.HeldDown = *message.Data;
}

void ServerReceive(OPprotocolSimpleMessage message) {
	OPlog("SERVER RECEIVED MESSAGE");
	_GS_EXAMPLE_SERVER_CLIENT.HeldDown = *message.Data;
	OPnetworkProtocolSimpleSend(
		_GS_EXAMPLE_SERVER_CLIENT.Protocol,
		NULL,
		(i8*)&_GS_EXAMPLE_SERVER_CLIENT.HeldDown,
		sizeof(i8)
		);
}



// This is for the Example Selector only
OPint GS_EXAMPLE_SERVER_CLIENT_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_SERVER_CLIENT = &_GS_EXAMPLE_SERVER_CLIENT;
