#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkAddress {
    sockaddr sockAddr;


    const OPchar* networkAddress;
    ui32 networkPort = 0;
    OPchar networkPortStr[6];
    OPnetworkSocketType::Enum networkSocketType;
    OPnetworkFamily::Enum networkFamily;
    
	bool valid;

    OPnetworkAddress() { }
    OPnetworkAddress(const OPchar* address, ui32 port, OPnetworkFamily::Enum familyType) {
        Init(address, port, familyType);
    }
    OPnetworkAddress(ui32 port, OPnetworkFamily::Enum familyType) {
        Init(port, familyType);
    }

    OPnetworkAddress(sockaddr_in* sockaddr) {
        Init(sockaddr);
    }

    void Init(sockaddr_in* sockaddr);
    void Init(ui32 port, OPnetworkFamily::Enum familyType);
    void Init(const OPchar* address, ui32 port, OPnetworkFamily::Enum familyType);

    inline sockaddr_in* SockAddrIn() { return (sockaddr_in*)&sockAddr; }
};