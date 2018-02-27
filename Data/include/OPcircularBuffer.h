#pragma once

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

struct OPcircularBuffer {
    i8* buffer = NULL;
    i32 pos = 0;
    i32 capacity = 0;
    i32 written = 0;

    inline void Init(void* b, ui32 s) {
        buffer = (i8*)b;
        capacity = s;
    }

    inline i8 I8() {
        i8 result = buffer[pos];
        pos++;
        written++;
        if(pos > capacity) {
            pos = 0;
        }
        return result;
    }

    inline void I8(i8 v) {
        buffer[pos] = v;
        pos++;
        written++;
        if(pos > capacity) {
            pos = 0;
        }
    }

    inline void Write(void* data, ui32 c) {
        ui32 sizeToEnd = capacity - pos;
        if(sizeToEnd < c) {
            // write first half
            ui32 toWrite = sizeToEnd - c;
            OPmemcpy(&buffer[pos], data, toWrite);
            c -= toWrite;
            pos = 0;
            OPmemcpy(&buffer[pos], data, c);
        } else {
            OPmemcpy(&buffer[pos], data, c);
        }
        written += c;
    }

    inline void Read(void* data, ui32 max) {
        // read from beginning
        i32 beginning = pos - written;
        if(beginning < 0) {
            beginning += capacity;
        }

        ui32 sizeToEnd = capacity - beginning;
        ui32 toRead = written;
        if(written > max) {
            toRead = max;
        }

        if(toRead < sizeToEnd) {
            OPmemcpy(data, &buffer[beginning], toRead);
            return;
        }

        OPmemcpy(data, &buffer[beginning], sizeToEnd);
        beginning = 0;
        toRead -= sizeToEnd;
        OPmemcpy(data, &buffer[beginning], toRead);
    }

};