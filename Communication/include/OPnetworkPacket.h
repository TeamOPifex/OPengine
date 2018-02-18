#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkPacket {
	i8 buffer[1024];
	ui16 size;
	ui16 pos;
	
	inline i8 I8() {
		return buffer[pos++];
	}

	inline void I8(i8 c) {
		buffer[pos++] = c;
	}
	
	inline i16 I16() {
		i16 val = *(i16*)&buffer[pos];
		i16 result = ntohs(val);
		pos += 2;
		return result;
	}

	inline void I16(i16 s) {
		i16 val = htons(s);
		*(i16*)&buffer[pos] = val;
		pos += 2;
	}
};