#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Communication/include/OPsimpleProtocol.h"

#ifdef OPIFEX_OPTION_RAKNET
#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
#endif

// Data for this Game State Example
class RakNetExample : public OPgameState {
	i8 Mode;
	i8 HeldDown;
#ifdef OPIFEX_OPTION_RAKNET
	RakNet::RakPeerInterface* server;
	RakNet::RakPeerInterface* client;
	RakNet::RakNetStatistics* rss;	
	
	RakNet::Packet* p;
	ui8 packetIdentifier;
	RakNet::SystemAddress clientID;
#endif

	void Init(OPgameState* last) {
		Mode = 0;
		HeldDown = 0;
#ifdef OPIFEX_OPTION_RAKNET
		clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
#endif
	}
#ifdef OPIFEX_OPTION_RAKNET
	// Copied from Multiplayer.cpp
	// If the first byte is ID_TIMESTAMP, then we want the 5th byte
	// Otherwise we want the 1st byte
	unsigned char GetPacketIdentifier(RakNet::Packet *p)
	{
		if (p == 0)
			return 255;

		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
			return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char)p->data[0];
	}

	void ServerInit() {
		Mode = 1;

		const i8* portstring = "1337";

		OPlogInfo("Starting the server");

		server = RakNet::RakPeerInterface::GetInstance();
		server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
		server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

		RakNet::SocketDescriptor socketDescriptors[2];
		socketDescriptors[0].port = atoi(portstring);
		socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
		socketDescriptors[1].port = atoi(portstring);
		socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6

		bool b = server->Startup(4, socketDescriptors, 2) == RakNet::RAKNET_STARTED;
		server->SetMaximumIncomingConnections(4);

		if (!b)
		{
			OPlogErr("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

			// Try again, but leave out IPV6
			b = server->Startup(4, socketDescriptors, 1) == RakNet::RAKNET_STARTED;
			if (!b)
			{
				OPlogErr("Server failed to start.");

				Mode = 0;
				return;
			}
		}
		server->SetOccasionalPing(true);
		server->SetUnreliableTimeout(1000);

		DataStructures::List< RakNet::RakNetSocket2* > sockets;
		server->GetSockets(sockets);

		OPlogInfo("Socket addresses used by RakNet:\n");
		for (unsigned int i = 0; i < sockets.Size(); i++)
		{
			OPlogInfo("%i. %s\n", i + 1, sockets[i]->GetBoundAddress().ToString(true));
		}

		OPlogInfo("\nMy IP addresses:\n");
		for (unsigned int i = 0; i < server->GetNumberOfAddresses(); i++)
		{
			RakNet::SystemAddress sa = server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
			OPlogInfo("%i. %s (LAN=%i)\n", i + 1, sa.ToString(false), sa.IsLANAddress());
		}
	}

	void ServerUpdate() {
		i8 message[1024];

		// Update the server
		RakSleep(30);

		if (OPkeyboardWasReleased(OPkeyboardKey::T)) {
			const i8* msg = "Test 1337 message";

			OPlogInfo("Sending Test Message: %s", msg);

			server->Send(msg, strlen(msg) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		for (p = server->Receive(); p; server->DeallocatePacket(p), p = server->Receive())
		{
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));;
				break;


			case ID_NEW_INCOMING_CONNECTION:
				// Somebody connected.  We have their IP now
				printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				clientID = p->systemAddress; // Record the player ID of the client

				printf("Remote internal IDs:\n");
				for (int index = 0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
				{
					RakNet::SystemAddress internalId = server->GetInternalID(p->systemAddress, index);
					if (internalId != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
					{
						printf("%i. %s\n", index + 1, internalId.ToString(true));
					}
				}

				break;

			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;

			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));;
				break;

			default:
				// The server knows the static data of all clients, so we can prefix the message
				// With the name data
				printf("%s\n", p->data);

				// Relay the message.  We prefix the name for other clients.  This demonstrates
				// That messages can be changed on the server before being broadcast
				// Sending is the same as before
				sprintf(message, "%s", p->data);
				server->Send(message, (const int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);

				break;
			}

		}
	}

	void ClientInit() {
		i8 message[1024];

		Mode = 2;
		client = RakNet::RakPeerInterface::GetInstance();

		const i8* ip = "127.0.0.1";
		const i8* clientPortString = "1338";
		const i8* serverPortString = "1337";


		RakNet::SocketDescriptor socketDescriptor(atoi(clientPortString), 0);
		socketDescriptor.socketFamily = AF_INET;
		client->Startup(8, &socketDescriptor, 1);
		client->SetOccasionalPing(true);

		RakNet::ConnectionAttemptResult car = client->Connect(ip, atoi(serverPortString), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
		RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);

		OPlogInfo("\nMy IP addresses:\n");
		unsigned int i;
		for (i = 0; i < client->GetNumberOfAddresses(); i++)
		{
			OPlogInfo("%i. %s\n", i + 1, client->GetLocalIP(i));
		}

		OPlogInfo("My GUID is %s\n", client->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());


	}

	void ClientUpdate() {

		if (OPkeyboardWasReleased(OPkeyboardKey::T)) {
			const i8* msg = "Test 1337 message";

			OPlogInfo("Sending Test Message: %s", msg);

			client->Send(msg, strlen(msg) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}


		for (p = client->Receive(); p; client->DeallocatePacket(p), p = client->Receive())
		{
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;
			default:
				// It's a client, so just show the message
				printf("%s\n", p->data);
				break;
			}
		}
	}
#endif

	OPint Update(OPtimer* time) {

		////////////////////////
		// Update
		////////////////////////
#ifdef OPIFEX_OPTION_RAKNET
		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		if (Mode == 0 && OPkeyboardWasReleased(OPkeyboardKey::S)) {
			ExampleRakNetServerInit();
		}

		if (Mode == 0 && OPkeyboardWasReleased(OPkeyboardKey::C)) {
			ExampleRakNetClientInit();
		}

		if (Mode == 1) {
			ExampleRakNetServerUpdate();
		}

		if (Mode == 2) {
			ExampleRakNetClientUpdate();
		}
#endif

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


// This is for the Example Selector only
#ifdef OPIFEX_OPTION_RAKNET
OPint GS_EXAMPLE_RAKNET_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_RAKNET_AVAILABLE = 0;
#endif

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy

RakNetExample _GS_EXAMPLE_RAKNET;
OPgameState* GS_EXAMPLE_RAKNET = &_GS_EXAMPLE_RAKNET;