#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/OPnetworkPlatform.h"

i32 OPnetworkFamilyTypeToCode(OPnetworkFamily::Enum networkFamily) {
    switch(networkFamily) {
        case OPnetworkFamily::INET:
            return AF_INET;
        case OPnetworkFamily::INET6:
            return AF_INET6;
        default:
            return AF_UNSPEC;
    }
    return AF_UNSPEC;
}
