#include "./Data/include/OPcircularBuffer.h"

#include "./Core/include/OPlog.h"


void OPcircularBuffer::Init(i8* b, ui32 s) {
    buffer = b;
    capacity = s;
}

i8 OPcircularBuffer::I8() {
    i8 result = buffer[pos];
    pos++;
    size--;
    if(pos >= capacity) {
        pos = 0;
    }
    return result;
}

void OPcircularBuffer::I8(i8 v) {
    buffer[pos] = v;
    pos++;
    size++;
    if(pos >= capacity) {
        pos = 0;
    }
}

ui8 OPcircularBuffer::UI8() {
    ui8 result = ((ui8*)buffer)[pos];
    pos++;
    size--;
    if(pos >= capacity) {
        pos = 0;
    }
    return result;
}

void OPcircularBuffer::UI8(ui8 v) {
    ((ui8*)buffer)[pos] = v;
    pos++;
    size++;
    if(pos >= capacity) {
        pos = 0;
    }
}

// Writes a buffer into the circular buffer
void OPcircularBuffer::Write(i8* data, ui32 toWrite) {
    ui32 sizeToEnd = capacity - pos;

    if(toWrite < sizeToEnd) {
        OPmemcpy(&buffer[pos], data, toWrite);
        pos += toWrite;
    } else {
        // write first half
        ui32 firstHalf = sizeToEnd - 1;
        OPmemcpy(&((i8*)buffer)[pos], data, firstHalf);

        // write second half
        ui32 backHalf = toWrite - firstHalf;
        OPmemcpy(&buffer[0], &data[firstHalf], backHalf);

        pos = backHalf;
    }

    size += toWrite;
}

// Read the circular buffer into a buffer
// Max can either be the size to read, or the max
// amount to pull out (ex size of a buffer)
void OPcircularBuffer::Read(i8* data, ui32 max) {
    ui32 sizeToEnd = capacity - pos;
    ui32 toRead = size;
    if(size > max) {
        toRead = max;
    }

    if(toRead < sizeToEnd) {
		//OPlogInfo("Mem copy into %p at pos %d of %d", data, pos, toRead);
		//OPlogInfo("My buffer %p", &buffer[pos]);
        OPmemcpy(data, &buffer[pos], toRead);
        pos += toRead;
        size -= toRead;
        return;
    }

    // memcpy 1st half
    ui32 firstHalf = sizeToEnd - 1;
    OPmemcpy(data, &buffer[pos], firstHalf);

    // memcpy 2nd half
    ui32 backHalf = toRead - firstHalf; // remaining to copy
    OPmemcpy(&data[firstHalf], &buffer[0], backHalf);

    pos = backHalf;
    size -= toRead;
}

// Resets the position to the beginning of the known data set
// this way we can read back out of the buffer
void OPcircularBuffer::Rewind() {
    posAtRewind = pos;
    sizeAtRewind = size;
    pos -= size;
    if(pos < 0) {
        pos += (capacity - 1);
    }
}

void OPcircularBuffer::Write(OPcircularBuffer* buf) {
    ui32 s = buf->size;
    ui32 sizeToEnd = capacity - pos;
    if(s < sizeToEnd) {
		i8* bufInto = &buffer[pos];
		//OPlogInfo("Buffer to read into %p", bufInto);

        buf->Read(bufInto, s);

        pos += s;
    } else {
        ui32 firstHalf = sizeToEnd - 1;
        buf->Read(&buffer[pos], firstHalf);

        ui32 backHalf = s - firstHalf;
        buf->Read(&buffer[0], backHalf);
        pos = backHalf;
    }
    size += s;
}