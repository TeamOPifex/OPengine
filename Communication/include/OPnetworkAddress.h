#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkAddress {
    sockaddr addr;
    addrinfo* addressInfo;

    OPchar networkAddressStr[128];
    ui32 networkAddress = 0;
    ui32 networkPort = 0;
    OPchar networkPortStr[6];
    OPnetworkSocketType::Enum networkSocketType;
    OPnetworkFamily::Enum networkFamily;
    
	bool valid;

    OPnetworkAddress() { }
    OPnetworkAddress(const OPchar* address, ui32 port, OPnetworkProtocolType::Enum protocol) {
        Init(address, port, protocol);
    }
    OPnetworkAddress(ui32 port, OPnetworkProtocolType::Enum protocol) {
        Init(port, protocol);
    }

    OPnetworkAddress(sockaddr* sockaddr, OPnetworkProtocolType::Enum protocol) {
        Init(sockaddr, protocol);
    }

    void Init(sockaddr* sockaddr, OPnetworkProtocolType::Enum protocol);
    void Init(ui32 port, OPnetworkProtocolType::Enum protocol);
    void Init(const OPchar* address, ui32 port, OPnetworkProtocolType::Enum protocol);

    bool Match(sockaddr_storage* addr);
    void Destroy();
};