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

void MessageReceivedHandler(OPnetworkSocket* socket, void* data, ui32 size);
void MessageClientReceivedHandler(void* data, ui32 size);
void ClientConnectedHandler(OPnetworkSocket* socket);
void ClientDisconnectedHandler(OPnetworkSocket* socket);

// Data for this Game State Example
class ServerClientExample : public OPgameState {
public:
	i8 Mode;
	i8 HeldDown;
	
	OPnetworkClient networkClient;
	OPnetworkServer networkServer;
	
	OPchar* messageQueue[1024];
	ui32 messageQueueIndex = 0;

	OPchar port[6];
	OPchar serverPort[6];
	OPchar server[200];
	OPchar message[128];
	bool sendClick;


	void Init(OPgameState* last) {
		Mode = 0;
		HeldDown = 0;
		sendClick = false;
		message[0] = NULL;
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
			networkServer.Init(OPnetworkProtocolType::UDP, OPstringToNumber(port));
			networkServer.SetReceiveCallback(MessageReceivedHandler);
			networkServer.SetClientConnectedCallback(ClientConnectedHandler);
			networkServer.SetClientDisconnectedCallback(ClientDisconnectedHandler);
		}
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::C)) {
			// Client Mode
			Mode = 2;
			networkClient.Init(OPnetworkProtocolType::UDP, server, OPstringToNumber(serverPort));
			networkClient.SetReceiveCallback(MessageClientReceivedHandler);
		}

		if (Mode != 0 && OPMOUSE.WasReleased(OPmouseButton::LBUTTON)) {
			HeldDown = 0;
		}
#endif

		if(sendClick) {
			sendClick = false;
		}

		if (Mode == 1) {
			networkServer.Update();
		}

		if (Mode == 2) {
			networkClient.Update();
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

#ifdef ADDON_imgui
		OPimguiNewFrame();

		bool always = true;

		if (Mode == 0) {

			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Port", port, 6);
			if(ImGui::Button("Start Server")) {
				Mode = 1;
				networkServer.Init(OPnetworkProtocolType::UDP, OPstringToNumber(port));
				networkServer.SetReceiveCallback(MessageReceivedHandler);
				networkServer.SetClientConnectedCallback(ClientConnectedHandler);
				networkServer.SetClientDisconnectedCallback(ClientDisconnectedHandler);
			}
			ImGui::End();


			ImGui::SetNextWindowPos(ImVec2(300, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Server", server, 200);
			ImGui::InputText("Port", serverPort, 6);
			if(ImGui::Button("Connect")) {
				Mode = 2;
				networkClient.Init(OPnetworkProtocolType::UDP, server, OPstringToNumber(serverPort));
				networkClient.SetReceiveCallback(MessageClientReceivedHandler);
			}
			ImGui::End();
		}

		if (Mode == 1) { // Server
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Message", message, 128);
		
			if(ImGui::Button("Send")) {			
				HeldDown = 0;
				sendClick = true;
				networkServer.Send((void*)message, strlen(message) + 1);
			}

			ImGui::End();
		}


		if (Mode == 2) { // Server
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Message", message, 128);
		
			if(ImGui::Button("Send")) {
				HeldDown = 0;
				sendClick = true;
				networkClient.Send((void*)message, strlen(message) + 1);
			}

			ImGui::End();
		}

		if (Mode == 1 || Mode == 2) {
			ImGui::SetNextWindowPos(ImVec2(260, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Messages", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);

			for (ui32 i = messageQueueIndex; i--; i >= 0) {
				ImGui::Text(messageQueue[i]);
			}

			ImGui::End();
		}



		ImGui::Render();
#endif

		// Swaps the back buffer
		OPrenderPresent();
	}

	
	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {

		return 0;
	}
};

ServerClientExample _GS_EXAMPLE_SERVER_CLIENT;

void MessageReceivedHandler(OPnetworkSocket* socket, void* data, ui32 size) {
	OPlogInfo("Message %s", (i8*)data);
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = OPstringCopy((i8*)data);
}

void MessageClientReceivedHandler(void* data, ui32 size) {
	OPlogInfo("Message %s", (i8*)data);
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = OPstringCopy((i8*)data);
}

void ClientConnectedHandler(OPnetworkSocket* socket) {

}

void ClientDisconnectedHandler(OPnetworkSocket* socket) {

}

// This is for the Example Selector only
OPint GS_EXAMPLE_SERVER_CLIENT_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_SERVER_CLIENT = &_GS_EXAMPLE_SERVER_CLIENT;
