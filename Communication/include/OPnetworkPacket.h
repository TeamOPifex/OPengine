#pragma once

#include "./Core/include/OPtypes.h"

struct OPnetworkPacket {
	OPnetworkPacketRaw* RawPacket;
	i32 ClientId;
	OPchar* Message;
};