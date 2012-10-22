#pragma once
#ifndef OPEngine_Data_Stream
#define OPEngine_Data_Stream
#include "./../../Core/include/Core.h"

typedef struct{
	OPuint Length;   // bytes in this stream
	OPuint _pointer; // current location to write to
	ui8* Data;
} OPstream;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------------
OPstream* OPstreamCreate(OPuint size);
//-----------------------------------------------------------------------------
OPuint OPstreamDestroy(OPstream* stream);
//-----------------------------------------------------------------------------
OPuint OPwrite(OPstream* stream, void* data, OPuint size);
//-----------------------------------------------------------------------------
ui8* OPread(OPstream* stream, OPuint size);
//-----------------------------------------------------------------------------
OPuint OPseek(OPstream* stream, OPuint byte);
#ifdef __cplusplus
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{
	class OPStream{
		public:
			OPStream(OPuint size);
			~OPStream();
			OPuint Seek(OPuint byte);
			OPuint Write(void* data, OPuint size);
			ui8*   Read(OPuint size);
			ui8*   Data(){ return this->_stream->Data; }
			OPuint Length(){ return this->_stream->Length; }
		private:
			OPstream* _stream;
	};
}
}
#endif
#endif
