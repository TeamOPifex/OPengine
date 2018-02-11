#pragma once

#include "./Core/include/OPtypes.h"

struct OPnetworkFamily {
    enum Enum {
        INET,
        INET6,
        UNSPEC
        // Not supported
        // IPX
        // APPLETALK
    };
};

i32 OPnetworkFamilyTypeToCode(OPnetworkFamily::Enum networkFamily);