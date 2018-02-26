 #include "./Communication/include/OPnetworkPacket.h"

#include "./Core/include/OPmemory.h"
#include <string>

 void OPnetworkPacket::Str(const OPchar* str) {
    ui32 s = strlen(str) + 1;
    OPmemcpy(&buffer[pos], str, s);
    size += s;
    pos += s;
}	

OPchar* OPnetworkPacket::Str() {
    i8* b = &buffer[pos];
    pos += strlen(b) + 1;
    return b;
}