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
	#include <stdio.h>
#endif

typedef struct {
	FILE* file;
	ui32 start;
	ui32 length;
	int fileDescriptor;
} FileInformation;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	

FileInformation OPreadFileInformation(const char* path);


void OPfileInit(void* manager);

//-----------------------------------------------------------------------------
/**
 * OPwriteFile - Writes a byte stream to a file.
 *	@param path Location in the file system where the data will be written.
 *	@param data OPstream instance which will be written
 *	@return 1 on success, 0 on failure
 */
OPint OPwriteFile(const char* path, OPstream* data);
//-----------------------------------------------------------------------------
/**
 * OPreadFile - Reads a file into a byte stream.
 *	@param path Location where the data will be read from.
 *	@return pointer to the data stream, NULL on failure
 */
OPstream* OPreadFile(const char* path);
//-----------------------------------------------------------------------------
/**
 * OPfileExists - Checks for the existance of a file.
 *	@param path Location in the file system where we will look for a file.
 *	@return 1 on success, 0 on failure
 */
OPint OPfileExists(const char* path);
//-----------------------------------------------------------------------------
/**
 * OPdeleteFile - Deletes a file from the filesystem.
 *	@param path Location in the file system where we will look for a file.
 *	@return 1 on success, 0 on failure
 */
OPint OPdeleteFile(const char* path);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
};
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{
	class OPFile{
		public:
			static void OPfileInit(void* manager);
			static OPStream* Read(const char* path);
			static OPint Write(const char* path, OPStream* stream);
			static OPint Exists(const char* path);
			static OPint Delete(const char* path);
	};
}
}
#endif
#endif
