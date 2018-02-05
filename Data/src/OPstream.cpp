#include "./Data/include/OPstream.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPstring.h"
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

void OPstream::Init(OPuint size){
	Data = (ui8*)OPalloc(size);
	Length = size;
	Size = 0;
	_pointer = 0;
	Source = NULL;
	
	// no trash allowed
	OPbzero(Data, size);
}

void OPstream::Destroy() {
	if (Source != NULL) OPfree(Source);
	OPfree(Data);
}

OPuint OPstream::Write(void* data, OPuint size){
	OPuint len, ptr;
	ui8 *D, *d = (ui8*)data;
	ui8* nd;

	if(!data) return 0;

	len = Length;
	ptr = _pointer;
	D   = Data;

	// if we are out of space for this new element
	// double the size of the array
	if(ptr + size > len){
		// if the size of this element is greater
		// than the length, then modify the 'len' which it
		// will be resized to so that it will beable to contain
		// the element
		len = len > size ? len : size;

		// reallocate
		//OPlog("OPstream - resizing to %u\n", sizeof(ui8) * (len << 1));

		nd = (ui8*)OPrealloc(
			D,
			(sizeof(ui8) * len) << 1
		);

		if(errno == ENOMEM){
			OPlog("OPrealloc at %x failed to resize to %u bytes.",
#ifdef OPIFEX_ANDROID
				0,
#else
					(OPuint)D,
#endif
				(sizeof(ui8) * len) << 1
			);
			return -1;
		}

		// check to see if reallocation is successful
		if(nd){
			Data = nd;
			Length = len << 1;

			OPbzero(Data + ptr, ((len << 1) - ptr));
		}
		else
			return 0;
	}

	// copy new data into the stream
	//for(i = 0; i < size;)
	//	Data[ptr + i] = d[i++];
	//write(1,".",1);
	OPmemcpy(&Data[ptr], d, size);
	//write(1,"+",1);
	_pointer += size;

	if (Size < _pointer)
		Size = _pointer;

	return 1;
}
//-----------------------------------------------------------------------------
ui8* OPstream::Read(OPuint size){
	// retrieve data and pointer
	OPuint ptr = _pointer;
	ui8* data = Data + ptr;

	_pointer += size; // update pointer location
	return data;
}

void OPstream::ReadInto(ui8* buffer, OPuint size) {
	// retrieve data and pointer
	OPuint ptr = _pointer;
	ui8* data = Data + ptr;

	for (ui32 i = 0; i < size; i++) {
		buffer[i] = *data;
		data++;
	}

	_pointer += size; // update pointer location
	return;
}

//-----------------------------------------------------------------------------
ui8* OPstream::Peek(OPuint size) {
	// retrieve data and pointer
	OPuint ptr = _pointer;
	ui8* data = Data + ptr;
	return data;
}

void _fillBuffer(OPstream* stream) {
	OPchar* buffer = (OPchar*)(stream->Data + stream->_pointer);
#ifdef OPIFEX_WINDOWS
	sscanf_s(buffer, "%s", stream->Buffer, 128);
#else
	sscanf(buffer, "%s", stream->Buffer);
#endif
	stream->_pointer += strlen((OPchar*)stream->Buffer) + 1;
}

i8 OPstream::I8() {
	return *((i8*)Read(sizeof(i8)));
}
i16 OPstream::I16() {
	return *((i16*)Read(sizeof(i16)));
}
i32 OPstream::I32() {
	return *((i32*)Read(sizeof(i32)));
}
ui8 OPstream::UI8() {
	return *((ui8*)Read(sizeof(ui8)));
}
ui16 OPstream::UI16() {
	return *((ui16*)Read(sizeof(ui16)));
}
ui32 OPstream::UI32() {
	return *((ui32*)Read(sizeof(ui32)));
}
f32 OPstream::F32() {
	return *((f32*)Read(sizeof(f32)));
}


i8 OPstream::I8Peek() {
	return *((i8*)Peek(sizeof(i8)));
}
i16 OPstream::I16Peek() {
	return *((i16*)Peek(sizeof(i16)));
}
i32 OPstream::I32Peek() {
	return *((i32*)Peek(sizeof(i32)));
}
ui8 OPstream::UI8Peek() {
	return *((ui8*)Peek(sizeof(ui8)));
}
ui16 OPstream::UI16Peek() {
	return *((ui16*)Peek(sizeof(ui16)));
}
ui32 OPstream::UI32Peek() {
	return *((ui32*)Peek(sizeof(ui32)));
}
f32 OPstream::F32Peek() {
	return *((f32*)Peek(sizeof(f32)));
}

OPchar* OPstream::String() {
	ui32 j = 0, length = UI32();
	if (length == 0) return NULL;
	OPchar* name = (OPchar*)OPalloc(length + 1);
	for (; j < length; j++) {
		name[j] = I8();
	}
	name[length] = '\0';
	return name;
}

OPchar* OPstream::ReadLine() {
	OPchar buffer[512];
	i32 i;
	i32 pos;
	OPchar* result;
	OPchar c;

	pos = 0;
	i = 0;

	// Read until we find a newline \n
	// Or until we hit 512 chars
	do {
		c = ((OPchar*)(Data + _pointer))[i];
		i++;
		if(c == '\r') {
			pos--;
		} else {
			if(c != '\n') buffer[pos++] = c;
			if(pos > 512) break;
		}
	} while(c != '\n');

	result = (OPchar*)OPalloc(sizeof(OPchar) * i);
	OPmemcpy(result, buffer, sizeof(OPchar) * (i - 1));
	result[i - 1] = '\0';

	_pointer += i;

	return result;
}

OPint OPstream::ReadKeyValuePair(struct OPkeyValuePair* dst){
	OPchar buffer[520];
	OPchar buffer2[255];
	OPint len;
	i32 i;

	// check to see if we are at the end of the stream or not
	if(_pointer >= Length) return 0;

#ifdef OPIFEX_WINDOWS
	sscanf_s((OPchar*)Data + _pointer, "%520[^\n]", buffer, 520);
#else
	sscanf((OPchar*)Data + _pointer, "%520[^\n]", buffer);
#endif
	len = strlen(buffer);
	OPlog("OPstreamReadKeyValuePair() - buffer: '%s'", buffer);

	if(!len) return 0;

	_pointer += len + 1;
#ifdef OPIFEX_WINDOWS
	sscanf_s(buffer, "%255[^ =]%*[= \t]%255[^\r\n]", buffer2, 255, dst->value, 255);
#else
	sscanf(buffer, "%255[^ =]%*[= \t]%255[^\r\n]", buffer2, dst->value);
#endif

	OPlog("OPstreamReadKeyValuePair() - '%s' = '%s'", buffer2, dst->value);

	// Removes any trailing whitespace from t,he values
#ifdef OPIFEX_WINDOWS
	sscanf_s(buffer2, "%s", dst->key, 255);
#else
	sscanf(buffer2, "%s", dst->key);
#endif

	for (i = 0; i < strlen(dst->key); i++){
		dst->key[i] = tolower(dst->key[i]);
	}

	return 1;
}

ui8* OPstream::ReadAt(OPuint pos, OPuint size){
	return Data + pos;
}

OPuint OPstream::Copy(void* dest, OPuint size){
	OPuint ptr = _pointer;
	OPuint oldSize = size;

	// truncate the number of bytes that will be returned
	// in the event that we are near the end of th stream
	if(Length <= ptr + size){
		size = (Length - ptr) - 1;
	}

	// ensure the size did not underflow
	if(size <= oldSize) return -1;

	// copy the data, and update the pointer location
	OPmemcpy(dest, Data + ptr, size);
	_pointer += size;

	return size;
}

OPuint OPstream::Seek(OPuint byte){
	if(byte < Length){
		_pointer = byte;
		return 1;
	}
	return 0;
}

OPuint OPstream::Offset(OPuint byte) {
	if (_pointer + byte < Length) {
		_pointer += byte;
		return 1;
	}
	return 0;
}