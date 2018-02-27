#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

#ifdef ADDON_imgui
#include "./OPimgui.h"
#endif

struct MessageType {
	enum Enum {
		PositionUpdate = 1,
		Message = 2,
		PlayerIndex = 3
	};
};

#include "ServerClient.h"
OPclientProtocol CLIENT_PROTOCOL;
OPserverProtocol SERVER_PROTOCOL;

OPnetworkProtocolType::Enum protocolType = OPnetworkProtocolType::UDP;
//OPnetworkProtocolType::Enum protocolType = OPnetworkProtocolType::TCP;

#define SERVER_CLIENT_PLAYER_MAX 10

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

	i8 myPlayerInd = -1;
	ui32 playerInd = 0;
	OPvec3 positions[SERVER_CLIENT_PLAYER_MAX];
	OPmodel* Mesh;
	OPeffect Effect;
	OPcam Camera;
	OPtexture* Texture;


	void Init(OPgameState* last) {
		Mode = 0;
		HeldDown = 0;
		sendClick = false;
		message[0] = NULL;
		OPmemcpy(port, "1337", 5);
		OPmemcpy(server, "127.0.0.1", 10);
		OPmemcpy(serverPort, "1337", 5);

		for(ui32 i = 0; i < SERVER_CLIENT_PLAYER_MAX; i++) {
			positions[i] = OPVEC3_ZERO;
		}


		OPCMAN.Load("Common/Texture.frag");
		OPCMAN.Load("Common/Texture3D.vert");
		const OPchar* _model = "swordsman.opm";
		const OPchar* _texture = "swordsman.png";
		Mesh = (OPmodel*)OPCMAN.LoadGet(_model);
		Texture = (OPtexture*)OPCMAN.LoadGet(_texture);
		
		OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
		OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
		Effect.Init(vert, frag);
		Effect.AddUniform("uColorTexture");
		//Effect->AddUniform("vLightDirection");
		Effect.AddUniform("uWorld");
		Effect.AddUniform("uProj");
		Effect.AddUniform("uView");

		Mesh->vertexLayout.SetOffsets(&Effect);
		Mesh->vertexArray.SetLayout(&Mesh->vertexLayout);

		Camera.SetPerspective(
			OPVEC3_ONE * 50.0,
			OPvec3Create(0, 0, 0),
			OPvec3Create(0, 1, 0),
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);
		OPrenderDepth(1);
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
			networkServer.Init(protocolType, OPstringToNumber(port));
		}
		if (Mode == 0 && OPKEYBOARD.WasReleased(OPkeyboardKey::C)) {
			// Client Mode
			Mode = 2;
			networkClient.Init(protocolType, server, OPstringToNumber(serverPort));
		}

		if (Mode != 0 && OPMOUSE.WasReleased(OPmouseButton::LBUTTON)) {
			HeldDown = 0;
		}
#endif

		if(sendClick) {
			sendClick = false;
		}

		if (Mode == 1) {
			networkServer.Update(time->Elapsed);
		}

		if (Mode == 2) {
			networkClient.Update();
		}

		bool sendPos = false;
		if(myPlayerInd > -1) {
			if (Mode == 1 || Mode == 2) {
				if(OPKEYBOARD.IsDown(OPkeyboardKey::W)) {
					positions[myPlayerInd].y+=0.5f;
					sendPos = true;
				}
				if(OPKEYBOARD.IsDown(OPkeyboardKey::S)) {
					positions[myPlayerInd].y-=0.5f;
					sendPos = true;
				}
				if(OPKEYBOARD.IsDown(OPkeyboardKey::D)) {
					positions[myPlayerInd].x+=0.5f;
					sendPos = true;
				}
				if(OPKEYBOARD.IsDown(OPkeyboardKey::A)) {
					positions[myPlayerInd].x-=0.5f;
					sendPos = true;
				}
			}

			if(sendPos) {
				OPnetworkPacket packet;
				packet.I8(1); // pos update
				packet.F32( positions[myPlayerInd].x );
				packet.F32( positions[myPlayerInd].y );
				packet.F32( positions[myPlayerInd].z );
				if (Mode == 1) {
					networkServer.Send(&packet);
				} else {
					networkClient.Send(&packet);
				}
			}
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
		
		if (Mode == 1 || Mode == 2) {
			
			Effect.Bind();
			Mesh->Bind();
			OPeffectSet("uColorTexture", Texture, 0);
			OPeffectSet("uProj", &Camera.proj);
			OPeffectSet("uView", &Camera.view);
			OPrenderCullMode(OPcullFace::BACK);
			OPrenderDepth(true);
			OPrenderDepthWrite(true);

			for(ui32 i = 0; i < playerInd; i++) {
				OPmat4 world;
				world.SetTranslate(positions[i]);
				world.Scl(0.1f);
				OPeffectSet("uWorld", &world);
				OPrenderDrawBufferIndexed(0);
			}
			
		}

#ifdef ADDON_imgui
		OPimguiNewFrame();

		bool always = true;

		if (Mode == 0) {

			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Port", port, 6);
			if(ImGui::Button("Start Server")) {
				Mode = 1;
				networkServer.Init(protocolType, OPstringToNumber(port));
				OPnetworkState::Change(&SERVER_PROTOCOL);
			}
			ImGui::End();


			ImGui::SetNextWindowPos(ImVec2(300, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Server", server, 200);
			ImGui::InputText("Port", serverPort, 6);
			if(ImGui::Button("Connect")) {
				Mode = 2;
				networkClient.Init(protocolType, server, OPstringToNumber(serverPort));
				OPnetworkState::Change(&CLIENT_PROTOCOL);
			}
			ImGui::End();
		}

		if (Mode == 1) { // Server
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Server Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Message", message, 128);
			for(ui32 i = 0; i < playerInd; i++) {
				ImGui::InputFloat3("Pos", (f32*)&positions[i]);
			}
		
			if(ImGui::Button("Send")) {			
				HeldDown = 0;
				sendClick = true;
				OPnetworkPacket packet;
				packet.I8((i8)MessageType::Message);
				packet.Str(message);
				networkServer.Send(&packet);
			}

			ImGui::End();
		}


		if (Mode == 2) { // Client
			ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiSetCond_::ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Client Controls", &always, ImVec2(250, 475), -1.0F, ImGuiWindowFlags_NoResize);
			ImGui::InputText("Message", message, 128);
			for(ui32 i = 0; i < playerInd; i++) {
				ImGui::InputFloat3("Pos", (f32*)&positions[i]);
			}
		
			if(ImGui::Button("Send")) {
				HeldDown = 0;
				sendClick = true;
				OPnetworkPacket packet;
				packet.I8((i8)MessageType::Message);
				packet.Str(message);
				networkClient.Send(&packet);
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

// This is for the Example Selector only
OPint GS_EXAMPLE_SERVER_CLIENT_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_SERVER_CLIENT = &_GS_EXAMPLE_SERVER_CLIENT;




OPint OPserverProtocol::Init(OPnetworkState* prev) { return 1; }
void OPserverProtocol::Connected(OPnetworkSocket* socket) {
	OPuint playerInd = _GS_EXAMPLE_SERVER_CLIENT.playerInd;

	socket->userData = (void*)playerInd;
	_GS_EXAMPLE_SERVER_CLIENT.playerInd++;
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = "[Client Connected]";

	{
		// Tell client what their ID is
		OPnetworkPacket packet;
		packet.I8((i8)MessageType::PlayerIndex);
		packet.I8(playerInd);
		_GS_EXAMPLE_SERVER_CLIENT.networkServer.Send(socket, &packet);
		OPlogInfo("Sent Client their Player Index: %d", playerInd);
	}

	// send everyone's position
	for(ui32 i = 0; i < playerInd; i++) {
		OPnetworkPacket packet;
		packet.I8((i8)MessageType::PositionUpdate);
		packet.I8(i);
		packet.F32(_GS_EXAMPLE_SERVER_CLIENT.positions[i].x);
		packet.F32(_GS_EXAMPLE_SERVER_CLIENT.positions[i].y);
		packet.F32(_GS_EXAMPLE_SERVER_CLIENT.positions[i].z);
		_GS_EXAMPLE_SERVER_CLIENT.networkServer.Send(socket, &packet);
	}
}

void OPserverProtocol::Disconnected(OPnetworkSocket* socket) {
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = "[Client Disconnected]";
}
void OPserverProtocol::Message(OPnetworkSocket* socket, OPnetworkPacket* packet) {
	i8 c = packet->I8();
	if(c == (i8)MessageType::Message) {
		i8* str = packet->Str();
		OPlogInfo("Server Received from %d: %s", socket->networkID, str);
		_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = OPstringCopy(str);
	} else if(c == 1) { // position update
		i8 ind = (OPuint)socket->userData;
		f32 x = packet->F32();
		f32 y = packet->F32();
		f32 z = packet->F32();
		_GS_EXAMPLE_SERVER_CLIENT.positions[ind] = OPvec3(x,y,z);
		// tell everyone about this position update

		OPnetworkPacket packet;
		packet.I8((i8)MessageType::PositionUpdate);
		packet.I8(ind);
		packet.F32(x);
		packet.F32(y);
		packet.F32(z);
		_GS_EXAMPLE_SERVER_CLIENT.networkServer.Send(&packet);
	}
}
OPint OPserverProtocol::Exit(OPnetworkState* prev) { return 1; }




OPint OPclientProtocol::Init(OPnetworkState* prev) { return 1; }
void OPclientProtocol::Connected(OPnetworkSocket* socket) {
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = "[Connected]";
}
void OPclientProtocol::Disconnected(OPnetworkSocket* socket) {
	_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = "[Disconnected]";
}
void OPclientProtocol::Message(OPnetworkSocket* socket, OPnetworkPacket* packet) {
	i8 c = packet->I8();
	// OPlogInfo("Received a packet of type: %d", c);
	if(c == (i8)MessageType::PlayerIndex) { // Player ID
		_GS_EXAMPLE_SERVER_CLIENT.myPlayerInd = packet->I8();
		_GS_EXAMPLE_SERVER_CLIENT.playerInd = _GS_EXAMPLE_SERVER_CLIENT.myPlayerInd + 1;
		OPlogInfo("Received Player Index: %d", _GS_EXAMPLE_SERVER_CLIENT.myPlayerInd);
	} else if(c == (i8)MessageType::Message) {
		i8* str = packet->Str();
		OPlogInfo("Client Received from %d: %s", socket->networkID, str);
		_GS_EXAMPLE_SERVER_CLIENT.messageQueue[_GS_EXAMPLE_SERVER_CLIENT.messageQueueIndex++] = OPstringCopy(str);
	} else if(c == (i8)MessageType::PositionUpdate) {
		i8 ind = packet->I8();
		if(ind >= _GS_EXAMPLE_SERVER_CLIENT.playerInd) {
			_GS_EXAMPLE_SERVER_CLIENT.playerInd = ind + 1;
			OPlogInfo("Max Players updated to %d", _GS_EXAMPLE_SERVER_CLIENT.playerInd);
		}
		f32 x = packet->F32();
		f32 y = packet->F32();
		f32 z = packet->F32();
		if(ind != _GS_EXAMPLE_SERVER_CLIENT.myPlayerInd) {
			// don't bother updating "my" pos, it's probably already udpated past this
			_GS_EXAMPLE_SERVER_CLIENT.positions[ind] = OPvec3(x, y, z);
		}
	}
}
OPint OPclientProtocol::Exit(OPnetworkState* prev) { return 1; }