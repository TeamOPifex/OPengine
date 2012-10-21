#pragma once
#ifndef OPEngine_Data_Stream
#define OPEngine_Data_Stream
#include "../../Core/include/Core.h"
#include <stdio.h>

typedef struct{
	OPuint Length;   // bytes in this stream
	OPuint _pointer; // current location to write to
	ui8* Data;
} OPstream;
//-----------------------------------------------------------------------------
OPstream* OPstreamCreate(OPuint size){
	OPstream* s = (OPstream*)OPalloc(sizeof(OPstream));
	s->Data = (ui8*)OPalloc(size);
	s->Length = size;
	s->_pointer = 0;

	return s;
}
//-----------------------------------------------------------------------------
OPuint OPstreamDestroy(OPstream* stream){
	OPfree(stream->Data);
	OPfree(stream);

	return 1;
}
//-----------------------------------------------------------------------------
OPuint OPwrite(OPstream* stream, void* data, OPuint size){
	OPuint len, ptr, i;
	ui8 *D, *d = (ui8*)data;

	if(!stream || !data) return 0;

	len = stream->Length;
	ptr = stream->_pointer;
	D   = stream->Data;

	/*OPint* in = (OPint*)data;
	for(i = 0; i < size / sizeof(OPint); i++){
		printf("%d ", in[i]);
	}*/

	// if we are out of space for this new element
	// double the size of the array
	if(ptr + size > len){
		//printf("Resizing from %d to %d\n", len, len * 2);
		ui8* nd = (ui8*)OPrealloc(
			D,
			sizeof(ui8) * len * 2
		);

		// check to see if reallocation is successful
		if(nd){
			D = nd;
			stream->Length = len * 2;
		}
		else
			return -1;
	}

	// copy new data into the stream
	for(i = 0; i < size; D[ptr + i] = d[i++]);// printf("%c,", d[i-1]); printf("\n");
	stream->_pointer += size;

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
//-----------------------------------------------------------------------------
OPuint OPseek(OPstream* stream, OPuint byte){
	if(byte < stream->Length){	
		stream->_pointer = byte;
		return 1;
	}
	return 0;
}
#endif
