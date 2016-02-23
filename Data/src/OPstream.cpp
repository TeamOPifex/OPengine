#include "./Data/include/OPstream.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPstring.h"
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

//-----------------------------------------------------------------------------
OPstream* OPstreamCreate(OPuint size){
	OPstream* s = (OPstream*)OPalloc(sizeof(OPstream));
	s->Data = (ui8*)OPalloc(size);
	s->Length = size;
	s->Size = 0;
	s->_pointer = 0;

	// no trash allowed
	OPbzero(s->Data, size);

	return s;
}
//-----------------------------------------------------------------------------
OPuint OPstreamDestroy(OPstream* stream){
	if(!stream) return 0;
	OPfree(stream->Data);
	OPfree(stream);

	return 1;
}
//-----------------------------------------------------------------------------
OPuint OPwrite(OPstream* stream, void* data, OPuint size){
	OPuint len, ptr;
	ui8 *D, *d = (ui8*)data;
	ui8* nd;

	if(!stream || !data) return 0;

	len = stream->Length;
	ptr = stream->_pointer;
	D   = stream->Data;

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
				(OPuint)D,
				(sizeof(ui8) * len) << 1
			);
			return -1;
		}

		// check to see if reallocation is successful
		if(nd){
			stream->Data = nd;
			stream->Length = len << 1;

			OPbzero(stream->Data + ptr, ((len << 1) - ptr));
		}
		else
			return 0;
	}

	// copy new data into the stream
	//for(i = 0; i < size;)
	//	stream->Data[ptr + i] = d[i++];
	//write(1,".",1);
	OPmemcpy(&stream->Data[ptr], d, size);
	//write(1,"+",1);
	stream->_pointer += size;

	if (stream->Size < stream->_pointer)
		stream->Size = stream->_pointer;

	return 1;
}
//-----------------------------------------------------------------------------
ui8* OPread(OPstream* stream, OPuint size){
	// retrieve data and pointer
	OPuint ptr = stream->_pointer;
	ui8* data = stream->Data + ptr;

	stream->_pointer += size; // update pointer location
	return data;
}


void _fillBuffer(OPstream* stream) {
	sscanf((i8*)(stream->Data + stream->_pointer), "%s", stream->Buffer);
	stream->_pointer += strlen(stream->Buffer) + 1;
}

i8 OPstreamI8(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
i16 OPstreamI16(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
i32 OPstreamI32(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
ui8 OPstreamUI8(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
ui16 OPstreamUI16(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
ui32 OPstreamUI32(OPstream* stream) {
	_fillBuffer(stream);
	return atoi(stream->Buffer);
}
f32 OPstreamf32(OPstream* stream) {
	_fillBuffer(stream);
	return atof(stream->Buffer);
}
OPchar* OPstreamString(OPstream* stream) {
	_fillBuffer(stream);
	return OPstringCopy(stream->Buffer);
}






OPchar* OPstreamReadLine(OPstream* stream) {
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
		c = ((OPchar*)(stream->Data + stream->_pointer))[i];
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

	stream->_pointer += i;

	return result;
}





//-----------------------------------------------------------------------------
OPint OPstreamReadKeyValuePair(OPstream* str, struct OPkeyValuePair* dst){
	OPchar buffer[520];
	OPchar buffer2[255];
	OPint len;
	i32 i;

	// check to see if we are at the end of the stream or not
	if(str->_pointer >= str->Length) return 0;

	sscanf((OPchar*)str->Data + str->_pointer, "%520[^\n]", buffer);
	len = strlen(buffer);
	OPlog("OPstreamReadKeyValuePair() - buffer: '%s'", buffer);

	if(!len) return 0;

	str->_pointer += len + 1;
	sscanf(buffer, "%255[^ =]%*[= \t]%255[^\r\n]", buffer2, dst->value);

	OPlog("OPstreamReadKeyValuePair() - '%s' = '%s'", buffer2, dst->value);

	// Removes any trailing whitespace from t,he values
	sscanf(buffer2, "%s", dst->key);

	for (i = 0; i < strlen(dst->key); i++){
		dst->key[i] = tolower(dst->key[i]);
	}

	return 1;
}

//-----------------------------------------------------------------------------
ui8* OPreadAt(OPstream* stream, OPuint pos, OPuint size){
	return stream->Data + pos;
}
//-----------------------------------------------------------------------------
OPuint OPcopy(OPstream* stream, void* dest, OPuint size){
	OPuint ptr = stream->_pointer;
	OPuint oldSize = size;

	// truncate the number of bytes that will be returned
	// in the event that we are near the end of th stream
	if(stream->Length <= ptr + size){
		size = (stream->Length - ptr) - 1;
	}

	// ensure the size did not underflow
	if(size <= oldSize) return -1;

	// copy the data, and update the pointer location
	OPmemcpy(dest, stream->Data + ptr, size);
	stream->_pointer += size;

	return size;
}
//-----------------------------------------------------------------------------
OPuint OPseek(OPstream* stream, OPuint byte){
	if(byte < stream->Length){
		stream->_pointer = byte;
		return 1;
	}
	return 0;
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // compile the C++ class
using namespace OPEngine::Data;

OPStream::OPStream(OPuint size){
	this->_stream = OPstreamCreate(size);
}

OPStream::~OPStream(){
	OPstreamDestroy(this->_stream);
}

OPuint OPStream::Seek(OPuint byte){
	OPseek(this->_stream, byte);
}

OPuint OPStream::Write(void* data, OPuint size){
	OPwrite(this->_stream, data, size);
}

ui8*   OPStream::Read(OPuint size){
	OPread(this->_stream, size);
}
#endif
