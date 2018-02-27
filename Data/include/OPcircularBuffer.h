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

    inline void Init(void* b, ui32 s) {
        buffer = (i8*)b;
        capacity = s;
    }

    inline i8 I8() {
        i8 result = buffer[pos];
        pos++;
        size--;
        if(pos > capacity) {
            pos = 0;
        }
        return result;
    }

    inline void I8(i8 v) {
        buffer[pos] = v;
        pos++;
        size++;
        if(pos > capacity) {
            pos = 0;
        }
    }

    inline ui8 UI8() {
        ui8 result = ((ui8*)buffer)[pos];
        pos++;
        size--;
        if(pos > capacity) {
            pos = 0;
        }
        return result;
    }

    inline void UI8(ui8 v) {
        ((ui8*)buffer)[pos] = v;
        pos++;
        size++;
        if(pos > capacity) {
            pos = 0;
        }
    }

    inline i16 I16() {
        i16 data;
        Read(&data, 2);
        return data;
    }

    inline void I16(i16 v) {
        Write(&v, 2);
    }

    inline i32 I32() {
        i32 data;
        Read(&data, 4);
        return data;
    }

    inline void I32(i32 v) {
        Write(&v, 4);
    }

    inline ui32 UI32() {
        ui32 data;
        Read(&data, 4);
        return data;
    }

    inline void UI32(ui32 v) {
        Write(&v, 4);
    }

    // Writes a buffer into the circular buffer
    inline void Write(void* data, ui32 toWrite) {
        ui32 sizeToEnd = capacity - pos;

        if(toWrite < sizeToEnd) {
            OPmemcpy(&buffer[pos], data, toWrite);
            pos += toWrite;
        } else {
            // write first half
            OPmemcpy(&((i8*)buffer)[pos], data, sizeToEnd);

            // write second half
            ui32 remaining = toWrite - sizeToEnd;
            OPmemcpy(&((i8*)buffer)[0], &((i8*)data)[sizeToEnd], remaining);

            pos = remaining;
        }

        size += toWrite;
    }

    // Read the circular buffer into a buffer
    // Max can either be the size to read, or the max
    // amount to pull out (ex size of a buffer)
    inline void Read(void* data, ui32 max) {
        ui32 sizeToEnd = capacity - pos;
        ui32 toRead = size;
        if(size > max) {
            toRead = max;
        }

        if(toRead < sizeToEnd) {
            OPmemcpy(data, &((i8*)buffer)[pos], toRead);
            pos += toRead;
            size -= toRead;
            return;
        }

        // memcpy 1st half
        OPmemcpy(data, &((i8*)buffer)[pos], sizeToEnd);
        ui32 remaining = toRead - sizeToEnd; // remaining to copy
        OPmemcpy(&((i8*)data)[sizeToEnd], &((i8*)buffer)[0], remaining);
        pos = remaining;
        size -= toRead;
    }

    // Resets the position to the beginning of the known data set
    // this way we can read back out of the buffer
    inline void Rewind() {
        posAtRewind = pos;
        sizeAtRewind = size;
        pos -= size;
        if(pos < 0) {
            pos += capacity;
        }
    }

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

    inline void Write(OPcircularBuffer* buf) {
        ui32 s = buf->size;
        ui32 sizeToEnd = capacity - pos;
        if(s < sizeToEnd) {
            buf->Read(&((i8*)buffer)[pos], s);
            pos += s;
        } else {
            buf->Read(&((i8*)buffer)[pos], sizeToEnd);
            ui32 remaining = s - sizeToEnd;
            buf->Read(&((i8*)buffer)[0], remaining);
            pos = remaining;
        }
        size += s;
    }
};