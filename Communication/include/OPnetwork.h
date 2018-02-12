#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"
#include "./Communication/include/Enums/OPnetworkOperation.h"
#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkData.h"
#include "./Communication/include/OPnetworkSocket.h"


typedef void(*OPnetworkClientConnectedCallback)(OPnetworkSocket*);
typedef void(*OPnetworkClientDisconnectedCallback)(OPnetworkSocket*);

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

struct OPnetwork {
	public:
		static i32 Initialize();
		static i32 Shutdown();
		static void LogError(const OPchar* message);
};