#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/OPnetworkPlatform.h"

#define MAX_PACKET_SIZE 1024

struct OPnetworkPacket {
	i8 buffer[MAX_PACKET_SIZE];
	ui16 size = 0;
	ui16 pos = 0;
	
	inline i8 I8() {
		return buffer[pos++];
	}

	inline void I8(i8 c) {
		size++;
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
		size += 2;
	}
	
	inline i32 I32() {
		i32 val = *(i32*)&buffer[pos];
		i32 result = ntohl(val);
		pos += 4;
		return result;
	}

	inline void I32(i32 s) {
		i32 val = htonl(s);
		*((i32*)(&buffer[pos])) = val;
		pos += 4;
		size += 4;
	}

	inline ui32 UI32() {
		return (ui32)I32();
	}

	inline void UI32(ui32 s) {
		ui32 val = htonl(s);
		*((ui32*)(&buffer[pos])) = val;
		pos += 4;
		size += 4;
	}

	void Str(const OPchar* str);
	OPchar* Str();

	inline void F32(f32 v) {
		ui32 bits = pack754(v, 32, 8);
		*((ui32*)(&buffer[pos])) = bits;
		pos += 4;
		size += 4;
	}

	inline f32 F32() {
		ui32 bits = *(ui32*)&buffer[pos];
		pos += 4;
		return unpack754(bits, 32, 8);
	}

	// inline void D64(d64 v) {
	// 	ui64 bits = pack754(v, 64, 11);
	// 	UI64(bits);
	// }

	// inline d64 D64() {
	// 	ui64 bits = *(ui64*)&buffer[pos];
	// 	pos += 8;
	// 	return unpack754(bits, 64, 11);
	// }
private:
	ui64 pack754(double long f, ui64 bits, ui64 expbits);
	double long unpack754(ui64 i, ui64 bits, ui64 expbits);
};