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
#include <share.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#endif


// Define platform independent aliases for reading/writing from sockets
#define OPREAD(fd, buff, count, readSuccessfully){\
	readSuccessfully = read(fd, buff, count);\
}\

#define OPWRITE(fd, buff, count, writtenSuccesfully){\
	writtenSuccesfully = write(fd, buff, count);\
}\

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
	
//-----------------------------------------------------------------------------
/**
 * OPread_ui16 - Reads the next 2 bytes from the file and converts to ui16
 *	@param file FILE* to the file stream
 *	@return ui16
 */
ui16 OPread_ui16(OPstream* str);
//-----------------------------------------------------------------------------
/**
 * OPread_ui32 - Reads the next 4 bytes from the file and converts to ui32
 *	@param file FILE* to the file stream
 *	@return ui32
 */
ui32 OPread_ui32(OPstream* str);
//-----------------------------------------------------------------------------
/**
 * OPread_i32 - Reads the next 4 bytes from the file and converts to i32
 *	@param file FILE* to the file stream
 *	@return i32
 */
i32 OPread_i32(OPstream* str);
//-----------------------------------------------------------------------------
/**
 * OPread_f32 - Reads the next 4 bytes from the file and converts to f32
 *	@param file FILE* to the file stream
 *	@return f32
 */
f32 OPread_f32(OPstream* str);
//-----------------------------------------------------------------------------
/**
 * OPreadFileInformation - Reads file information, getting start and length
 *	@param path Location in the file system where the data will be written.
 *	@return FileInformation
 */
FileInformation OPreadFileInformation(const char* path);
//-----------------------------------------------------------------------------
/**
 * OPfileInit - Initiates the file system.
 *	@param manager Location in the file system where the data will be written.
 */
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
