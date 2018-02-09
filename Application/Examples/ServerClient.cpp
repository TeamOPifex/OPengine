#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Communication/include/OPsimpleProtocol.h"

#ifdef ADDON_imgui
#include "./OPimgui.h"
#endif


void ClientReceive(OPprotocolSimpleMessage message);
void ServerReceive(OPprotocolSimpleMessage message);

// Data for this Game State Example
class ServerClientExample : public OPgameState {
public:
	i8 Mode;
	i8 HeldDown;
	OPnetworkProtocolSimple* Protocol;
	// OPnetwork* network;
	OPchar port[6];
	OPchar serverPort[6];
	OPchar server[200];
	bool sendClick;


	void Init(OPgameState* last) {
		Mode = 0;
		HeldDown = 0;
		Protocol = NULL;
		sendClick = false;
		port[0] = '1'; port[1] = '3'; port[2] = '3'; port[3] = '7'; port[4] = NULL;
		OPmemcpy(port, "1337", 5);
		OPmemcpy(server, "127.0.0.1", 10);
		OPmemcpy(serverPort, "1337", 5);
	}


	OPint Update(OPtimer* time) {

		////////////////////////
		// Update
		////////////////////////

#ifdef ADDON_imgui
#else
		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::S)) {
			// Server Mode
			Mode = 1;
			Protocol = OPnetworkProtocolSimpleCreate(
				OPnetworkType::OPNETWORK_SERVER
			);
			Protocol->Network->ServerStart("1337");
		}
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::C)) {
			// Client Mode
			Mode = 2;
			Protocol = OPnetworkProtocolSimpleCreate(
				OPnetworkType::OPNETWORK_CLIENT
			);
			Protocol->Network->ClientConnect("127.0.0.1", "1337");
		}

		if (Mode != 0 && OPMOUSE.WasReleased(OPmouseButton::LBUTTON)) {
			HeldDown = 0;
			OPnetworkProtocolSimpleSend(
				Protocol,
				time,
				(i8*)&HeldDown,
				sizeof(i8)
			);
		}
#endif

		if(sendClick) {

			OPnetworkProtocolSimpleSend(
				Protocol,
				time,
				(i8*)&HeldDown,
				sizeof(i8)
			);
			sendClick = false;
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

		return false;

	}
	void Render(OPfloat delta) {
#ifdef ADDON_imgui
		if (Mode == 1) {
			// It's in server mode
			OPrenderClear(1.0, 0.0, 0.0);
		}
		else if (Mode == 2) {
			// It's in client mode
			OPrenderClear(0.0, 0.0, 1.0);
		} else {
			OPrenderClear(0.1); 
		}
#else
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
#endif

		OPimguiNewFrame();

		bool always = true;

		if (Mode == 0) {

			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Port", port, 6);
			if(ImGui::Button("Start Server")) {
				Mode = 1;
				Protocol = OPnetworkProtocolSimpleCreate(
					OPnetworkType::OPNETWORK_SERVER
				);
				Protocol->Network->ServerStart(port);
			}
			ImGui::End();


			ImGui::SetNextWindowPos(ImVec2(300, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Server", server, 200);
			ImGui::InputText("Port", serverPort, 6);
			if(ImGui::Button("Connect")) {
				Mode = 2;
				Protocol = OPnetworkProtocolSimpleCreate(
					OPnetworkType::OPNETWORK_CLIENT
				);
				Protocol->Network->ClientConnect(server, serverPort);
			}
			ImGui::End();
		}

		if (Mode == 1) { // Server
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
		
			if(ImGui::Button("Click")) {			
				HeldDown = 0;
				sendClick = true;
			}

			ImGui::End();
		}


		if (Mode == 2) { // Server
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
		
			if(ImGui::Button("Click")) {			
				HeldDown = 0;
				sendClick = true;
			}

			ImGui::End();
		}

		ImGui::Render();

		// Swaps the back buffer
		OPrenderPresent();
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
