#pragma once

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

struct OPcircularBuffer {
    i8* buffer = NULL;
    i32 pos = 0;
    i32 capacity = 0;
    i32 size = 0;
    ui32 posAtRewind = 0;
    ui32 sizeAtRewind = 0;

    void Init(i8* b, ui32 s);

    i8 I8();
    void I8(i8 v);
    ui8 UI8();
    void UI8(ui8 v);    
    
    inline i8 PeekI8() {
        return buffer[pos];
    }

	inline i16 I16() {
		i16 data;
		Read((i8*)&data, 2);
		return data;
	}

	inline void I16(i16 v) {
		Write((i8*)&v, 2);
	}

	inline ui16 UI16() {
		ui16 data;
		Read((i8*)&data, 2);
		return data;
	}

	inline void UI16(ui16 v) {
		Write((i8*)&v, 2);
	}

    inline i32 I32() {
        i32 data;
        Read((i8*)&data, 4);
        return data;
    }

    inline void I32(i32 v) {
        Write((i8*)&v, 4);
    }

	inline ui32 UI32() {
		ui32 data;
		Read((i8*)&data, 4);
		return data;
	}

	inline void UI32(ui32 v) {
		Write((i8*)&v, 4);
	}

	inline ui32 UI64() {
		ui32 data;
		Read((i8*)&data, 8);
		return data;
	}

	inline void UI64(ui64 v) {
		Write((i8*)&v, 8);
	}

    // Writes a buffer into the circular buffer
    void Write(i8* data, ui32 toWrite);
    void Write(OPcircularBuffer* buf);

    // Read the circular buffer into a buffer
    // Max can either be the size to read, or the max
    // amount to pull out (ex size of a buffer)
    void Read(i8* data, ui32 max);

    // Resets the position to the beginning of the known data set
    // this way we can read back out of the buffer
    void Rewind();

    // Network packets use this after reading the packet
    // and finding not enough data was present yet
    inline void Unread() {
        size = sizeAtRewind;
    }

    // Repositions the circular buffer at the end of the known data
    inline void FastForward() {
        pos = posAtRewind;
    }

    inline void ReadPast(ui32 s) {
        pos += s;
        size -= s;
    }

    inline void Zero() {
        pos = 0;
        size = 0;
    }
};