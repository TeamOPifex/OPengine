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
typedef struct {
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

} RakNetExample;

RakNetExample rakNetExampleExample;

void ExampleRakNetEnter(OPgameState* last) {
	rakNetExampleExample.Mode = 0;
	rakNetExampleExample.HeldDown = 0;
#ifdef OPIFEX_OPTION_RAKNET
	rakNetExampleExample.clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
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

void ExampleRakNetServerInit() {
	rakNetExampleExample.Mode = 1;

	const i8* portstring = "1337";

	OPlogInfo("Starting the server");

	rakNetExampleExample.server = RakNet::RakPeerInterface::GetInstance();
	rakNetExampleExample.server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	rakNetExampleExample.server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = atoi(portstring);
	socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
	socketDescriptors[1].port = atoi(portstring);
	socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6

	bool b = rakNetExampleExample.server->Startup(4, socketDescriptors, 2) == RakNet::RAKNET_STARTED;
	rakNetExampleExample.server->SetMaximumIncomingConnections(4);

	if (!b)
	{
		OPlogErr("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

		// Try again, but leave out IPV6
		b = rakNetExampleExample.server->Startup(4, socketDescriptors, 1) == RakNet::RAKNET_STARTED;
		if (!b)
		{
			OPlogErr("Server failed to start.");

			rakNetExampleExample.Mode = 0;
			return;
		}
	}
	rakNetExampleExample.server->SetOccasionalPing(true);
	rakNetExampleExample.server->SetUnreliableTimeout(1000);

	DataStructures::List< RakNet::RakNetSocket2* > sockets;
	rakNetExampleExample.server->GetSockets(sockets);

	OPlogInfo("Socket addresses used by RakNet:\n");
	for (unsigned int i = 0; i < sockets.Size(); i++)
	{
		OPlogInfo("%i. %s\n", i + 1, sockets[i]->GetBoundAddress().ToString(true));
	}

	OPlogInfo("\nMy IP addresses:\n");
	for (unsigned int i = 0; i < rakNetExampleExample.server->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = rakNetExampleExample.server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		OPlogInfo("%i. %s (LAN=%i)\n", i + 1, sa.ToString(false), sa.IsLANAddress());
	}
}

void ExampleRakNetServerUpdate() {
	i8 message[1024];

	// Update the server
	RakSleep(30);

	if (OPkeyboardWasReleased(OPkeyboardKey::T)) {
		const i8* msg = "Test 1337 message";

		OPlogInfo("Sending Test Message: %s", msg);

		rakNetExampleExample.server->Send(msg, strlen(msg) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	for (rakNetExampleExample.p = rakNetExampleExample.server->Receive(); rakNetExampleExample.p; rakNetExampleExample.server->DeallocatePacket(rakNetExampleExample.p), rakNetExampleExample.p = rakNetExampleExample.server->Receive())
	{
		// We got a packet, get the identifier with our handy function
		rakNetExampleExample.packetIdentifier = GetPacketIdentifier(rakNetExampleExample.p);

		// Check if this is a network message packet
		switch (rakNetExampleExample.packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", rakNetExampleExample.p->systemAddress.ToString(true));;
			break;


		case ID_NEW_INCOMING_CONNECTION:
			// Somebody connected.  We have their IP now
			printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", rakNetExampleExample.p->systemAddress.ToString(true), rakNetExampleExample.p->guid.ToString());
			rakNetExampleExample.clientID = rakNetExampleExample.p->systemAddress; // Record the player ID of the client

			printf("Remote internal IDs:\n");
			for (int index = 0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
			{
				RakNet::SystemAddress internalId = rakNetExampleExample.server->GetInternalID(rakNetExampleExample.p->systemAddress, index);
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
			printf("Ping from %s\n", rakNetExampleExample.p->systemAddress.ToString(true));
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST from %s\n", rakNetExampleExample.p->systemAddress.ToString(true));;
			break;

		default:
			// The server knows the static data of all clients, so we can prefix the message
			// With the name data
			printf("%s\n", rakNetExampleExample.p->data);

			// Relay the message.  We prefix the name for other clients.  This demonstrates
			// That messages can be changed on the server before being broadcast
			// Sending is the same as before
			sprintf(message, "%s", rakNetExampleExample.p->data);
			rakNetExampleExample.server->Send(message, (const int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, rakNetExampleExample.p->systemAddress, true);

			break;
		}

	}
}

void ExampleRakNetClientInit() {
	i8 message[1024];

	rakNetExampleExample.Mode = 2;
	rakNetExampleExample.client = RakNet::RakPeerInterface::GetInstance();
	
	const i8* ip = "127.0.0.1";
	const i8* clientPortString = "1338";
	const i8* serverPortString = "1337";


	RakNet::SocketDescriptor socketDescriptor(atoi(clientPortString), 0);
	socketDescriptor.socketFamily = AF_INET;
	rakNetExampleExample.client->Startup(8, &socketDescriptor, 1);
	rakNetExampleExample.client->SetOccasionalPing(true);

	RakNet::ConnectionAttemptResult car = rakNetExampleExample.client->Connect(ip, atoi(serverPortString), "Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	RakAssert(car == RakNet::CONNECTION_ATTEMPT_STARTED);

	OPlogInfo("\nMy IP addresses:\n");
	unsigned int i;
	for (i = 0; i < rakNetExampleExample.client->GetNumberOfAddresses(); i++)
	{
		OPlogInfo("%i. %s\n", i + 1, rakNetExampleExample.client->GetLocalIP(i));
	}

	OPlogInfo("My GUID is %s\n", rakNetExampleExample.client->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());


}

void ExampleRakNetClientUpdate() {

	if (OPkeyboardWasReleased(OPkeyboardKey::T)) {
		const i8* msg = "Test 1337 message";

		OPlogInfo("Sending Test Message: %s", msg);

		rakNetExampleExample.client->Send(msg, strlen(msg) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}


	for (rakNetExampleExample.p = rakNetExampleExample.client->Receive(); rakNetExampleExample.p; rakNetExampleExample.client->DeallocatePacket(rakNetExampleExample.p), rakNetExampleExample.p = rakNetExampleExample.client->Receive())
	{
		// We got a packet, get the identifier with our handy function
		rakNetExampleExample.packetIdentifier = GetPacketIdentifier(rakNetExampleExample.p);

		// Check if this is a network message packet
		switch (rakNetExampleExample.packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_ALREADY_CONNECTED:
			// Connection lost normally
			printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", rakNetExampleExample.p->guid);
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
			printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", rakNetExampleExample.p->systemAddress.ToString(true), rakNetExampleExample.p->guid.ToString());
			printf("My external address is %s\n", rakNetExampleExample.client->GetExternalID(rakNetExampleExample.p->systemAddress).ToString(true));
			break;
		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", rakNetExampleExample.p->systemAddress.ToString(true));
			break;
		default:
			// It's a client, so just show the message
			printf("%s\n", rakNetExampleExample.p->data);
			break;
		}
	}
}
#endif

OPint ExampleRakNetUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////
#ifdef OPIFEX_OPTION_RAKNET
	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (rakNetExampleExample.Mode == 0 && OPkeyboardWasReleased(OPkeyboardKey::S)) {
		ExampleRakNetServerInit();
	}

	if (rakNetExampleExample.Mode == 0 && OPkeyboardWasReleased(OPkeyboardKey::C)) {
		ExampleRakNetClientInit();
	}

	if (rakNetExampleExample.Mode == 1) {
		ExampleRakNetServerUpdate();
	}

	if (rakNetExampleExample.Mode == 2) {
		ExampleRakNetClientUpdate();
	}
#endif

	////////////////////////
	// Render
	////////////////////////
	if(rakNetExampleExample.HeldDown == 1) {
		// Someone is holding down the left mouse button
		OPrenderClear(1.0, 1.0, 1.0);
	} else if(rakNetExampleExample.Mode == 0) {
		// Server and Client not yet started
		OPrenderClear(0.0, 0.0, 0.0);
	}
	else if(rakNetExampleExample.Mode == 1) {
		// It's in server mode
		OPrenderClear(1.0, 0.0, 0.0);
	}
	else if(rakNetExampleExample.Mode == 2) {
		// It's in client mode
		OPrenderClear(0.0, 0.0, 1.0);
	}

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}
void ExampleRakNetRender(OPfloat delta) {

}
// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleRakNetExit(OPgameState* next) {

	return 0;
}

// This is for the Example Selector only
#ifdef OPIFEX_OPTION_RAKNET
OPint GS_EXAMPLE_RAKNET_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_RAKNET_AVAILABLE = 0;
#endif

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_RAKNET = {
	ExampleRakNetEnter,
	ExampleRakNetUpdate,
	ExampleRakNetRender,
	ExampleRakNetExit
};
