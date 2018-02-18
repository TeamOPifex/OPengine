#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkAddress {
    sockaddr_in addr;
    addrinfo* addressInfo;

    OPchar networkAddressStr[128];
    ui32 networkAddress = 0;
    ui32 networkPort = 0;
    OPchar networkPortStr[6];
    OPnetworkFamily::Enum networkFamily;
    
	bool valid;

    OPnetworkAddress() { }
    OPnetworkAddress(const OPchar* address, ui32 port) {
        Init(address, port);
    }
    OPnetworkAddress(ui32 port) {
        Init(port);
    }

    OPnetworkAddress(sockaddr_in* sockaddr) {
        Init(sockaddr);
    }

    void Init(sockaddr_in* sockaddr);
    void Init(ui32 port);
    void Init(const OPchar* address, ui32 port);

    bool Match(sockaddr_storage* addr);
    void Destroy();
};