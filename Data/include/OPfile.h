#pragma once
#ifndef OPEngine_Data_File
#define OPEngine_Data_File

#include "OPstream.h"

#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------------
OPint OPwriteFile(const char* path, OPstream* data);
//-----------------------------------------------------------------------------
OPstream* OPreadFile(const char* path);
//-----------------------------------------------------------------------------
OPint OPfileExists(const char* path);
//-----------------------------------------------------------------------------
OPint OPdeleteFile(const char* path);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // use the C++ file object
namespace OPEngine{
namespace Data{
	class OPFile{
		public:
			static OPStream* Read(const char* path);
			static OPint Write(const char* path, OPstream* stream);
			static OPint Exists(const char* path);
			static OPint Delete(const char* path);
	};
}
}
#endif
#endif
