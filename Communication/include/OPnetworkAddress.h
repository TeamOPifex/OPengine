#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkAddress {
    const OPchar* networkAddress;
    const OPchar* networkPort;
    ui32 networkPortNum = 0;
    OPnetworkSocketType::Enum networkSocketType;
    OPnetworkFamily::Enum networkFamily;
    addrinfo* result;
    
	bool valid;

    OPnetworkAddress() { }
    OPnetworkAddress(const OPchar* address, const OPchar* port, OPnetworkSocketType::Enum socketType) {
        Init(address, port, socketType);
    }
    OPnetworkAddress(const OPchar* address, ui32 port, OPnetworkSocketType::Enum socketType) {
        Init(address, port, socketType);
    }

    void Init(const OPchar* address, const OPchar* port, OPnetworkSocketType::Enum socketType);
    void Init(const OPchar* address, ui32 port, OPnetworkSocketType::Enum socketType);
};