#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Data/include/OPcircularBuffer.h"

#define MAX_PACKET_SIZE 1024
#define MAX_PACKET_SIZE_DOUBLE 2048

struct OPnetworkPacket {
	i8 internal_buffer[MAX_PACKET_SIZE_DOUBLE];
	OPcircularBuffer buffer;
	OPchar strBuffer[256];

	OPnetworkPacket() {
		buffer.Init(&internal_buffer[0], MAX_PACKET_SIZE_DOUBLE);
	}
	
	inline i8 I8() {
		return buffer.I8();
	}

	inline void I8(i8 c) {
		buffer.I8(c);
	}
	
	inline ui8 UI8() {
		return buffer.UI8();
	}

	inline void UI8(ui8 c) {
		buffer.UI8(c);
	}

	inline i16 I16() {
		i16 val = buffer.I16();
		return ntohs(val);
	}

	inline void I16(i16 s) {
		i16 val = htons(s);
		buffer.I16(val);
	}

	inline ui16 UI16() {
		ui16 val = buffer.UI16();
		return ntohs(val);
	}

	inline void UI16(i16 s) {
		ui16 val = (ui16)htons(s);
		buffer.UI16(val);
	}
	
	inline i32 I32() {
		i32 val = buffer.I32();
		return ntohl(val);
	}

	inline void I32(i32 s) {
		i32 val = htonl(s);
		buffer.I32(val);
	}

	inline ui32 UI32() {
		return (ui32)I32();
	}

	inline void UI32(ui32 s) {
		ui32 val = htonl(s);
		buffer.UI32(val);
	}

	//inline ui64 UI64() {
	//	return (ui64)I64();
	//}

	//inline void UI64(ui64 s) {
	//	ui64 val = htonll(s);
	//	buffer.UI64(val);
	//}

	void Str(OPchar* str);
	OPchar* Str();

	inline void F32(f32 v) {
		ui32 bits = pack754(v, 32, 8);
		buffer.UI32(bits);
	}

	inline f32 F32() {
		ui32 bits = buffer.UI32();
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

	// ui64 htonll(ui64 value);
	ui64 pack754(double long f, ui64 bits, ui64 expbits);
	double long unpack754(ui64 i, ui64 bits, ui64 expbits);
};