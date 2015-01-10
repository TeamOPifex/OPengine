#ifndef OPEngine_Data_File
#define OPEngine_Data_File
#include "./Data/include/OPstream.h"

#include <stdio.h>

#if defined(OPIFEX_UNIX)
#include <unistd.h>
#elif defined(OPIFEX_WINDOWS)
#include <io.h>
#endif

// Define platform independent aliases for reading/writing from sockets and files
#define OPFILEREAD(fd, buff, count, readSuccessfully){\
	readSuccessfully = read(fd, buff, count);\
}\

#define OPFILEWRITE(fd, buff, count, writtenSuccesfully){\
	writtenSuccesfully = write(fd, buff, count);\
}\

typedef struct {
	OPint _handle;
} OPfile;

typedef struct {
	FILE* file;
	ui32 start;
	ui32 length;
	int fileDescriptor;
} OPfileInformation;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	
//-----------------------------------------------------------------------------
/**
 * OPreadui16 - Reads the next 2 bytes from the file and converts to ui16
 *	@param file FILE* to the file stream
 *	@return ui16
 */
	EXPORT_METHOD ui16 OPreadui16(OPstream* str);
//-----------------------------------------------------------------------------
/**
 * OPreadui32 - Reads the next 4 bytes from the file and converts to ui32
 *	@param file FILE* to the file stream
 *	@return ui32
 */
	EXPORT_METHOD ui32 OPreadui32(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadi8 - Reads the next 1 byte from the file and converts to i8
 *	@param file FILE* to the file stream
 *	@return i8
 */
	EXPORT_METHOD OPchar OPreadi8(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadi16 - Reads the next 2 bytes from the file and converts to i16
 *	@param file FILE* to the file stream
 *	@return i16
 */
	EXPORT_METHOD i16 OPreadi16(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadi32 - Reads the next 4 bytes from the file and converts to i32
 *	@param file FILE* to the file stream
 *	@return i32
 */
	EXPORT_METHOD i32 OPreadi32(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadf32 - Reads the next 4 bytes from the file and converts to f32
 *	@param file FILE* to the file stream
 *	@return f32
 */
	EXPORT_METHOD f32 OPreadf32(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadstring - Reads the next string from the file
 *	@param file FILE* to the file stream
 *	@return i8*
 */
	EXPORT_METHOD OPchar* OPreadstring(OPstream* str);

//-----------------------------------------------------------------------------
/**
 * OPreadFileInformation - Reads file information, getting start and length
 *	@param path Location in the file system where the data will be written.
 *	@return FileInformation
 */
	EXPORT_METHOD OPfileInformation OPreadFileInformation(const char* path);

//-----------------------------------------------------------------------------
/**
 * OPwriteFile - Writes a byte stream to a file.
 *	@param path Location in the file system where the data will be written.
 *	@param data OPstream instance which will be written
 *	@return 1 on success, 0 on failure
 */
	EXPORT_METHOD OPint OPwriteFile(const char* path, OPstream* data);

//-----------------------------------------------------------------------------
/**
 * OPreadFile - Reads a file into a byte stream.
 *	@param path Location where the data will be read from.
 *	@return pointer to the data stream, NULL on failure
 */
	EXPORT_METHOD OPstream* OPreadFile(const char* path);

//-----------------------------------------------------------------------------
/**
 * OPreadFile - Reads a file into a byte stream.
 *	@param path Location where the data will be read from.
 *	@return pointer to the data stream, NULL on failure
 */
	EXPORT_METHOD OPstream* OPreadFileLarge(const char* path, ui32 expectedSize);

//-----------------------------------------------------------------------------
/**
 * OPfileExists - Checks for the existance of a file.
 *	@param path Location in the file system where we will look for a file.
 *	@return 1 on success, 0 on failure
 */
	EXPORT_METHOD OPint OPfileExists(const char* path);

//-----------------------------------------------------------------------------
/**
 * OPdeleteFile - Deletes a file from the filesystem.
 *	@param path Location in the file system where we will look for a file.
 *	@return 1 on success, 0 on failure
 */
	EXPORT_METHOD OPint OPdeleteFile(const char* path);

	EXPORT_METHOD ui64 OPfileLastChange(const OPchar* path);

	EXPORT_METHOD OPfile OPfileOpen(const OPchar* path);
	EXPORT_METHOD OPint OPfileWriteui8(OPfile* file, ui8 data);
	EXPORT_METHOD OPint OPfileWriteui16(OPfile* file, ui16 data);
	EXPORT_METHOD OPint OPfileWriteui32(OPfile* file, ui32 data);
	EXPORT_METHOD OPint OPfileWritei8(OPfile* file, i8 data);
	EXPORT_METHOD OPint OPfileWritei16(OPfile* file, i16 data);
	EXPORT_METHOD OPint OPfileWritei32(OPfile* file, i32 data);
	EXPORT_METHOD OPint OPfileWriteString(OPfile* file, const OPchar* data);
	EXPORT_METHOD OPint OPfileWriteBytes(OPfile* file, void* data, ui64 bytesToWrite);
	EXPORT_METHOD ui8 OPfileReadui8(OPfile* file);
	EXPORT_METHOD ui16 OPfileReadui16(OPfile* file);
	EXPORT_METHOD ui32 OPfileReadui32(OPfile* file);
	EXPORT_METHOD i8 OPfileReadi8(OPfile* file);
	EXPORT_METHOD i16 OPfileReadi16(OPfile* file);
	EXPORT_METHOD i32 OPfileReadi32(OPfile* file);
	EXPORT_METHOD OPchar* OPfileReadString(OPfile* file);
	EXPORT_METHOD void* OPfileReadBytes(OPfile* file, ui64 bytesToRead);
	EXPORT_METHOD OPint OPfileSeekReset(OPfile* file);
	EXPORT_METHOD OPint OPfileSeek(OPfile* file, OPint pos);
	EXPORT_METHOD OPint OPfileClose(OPfile* file);
	EXPORT_METHOD OPint OPfileCopy(const OPchar* existing, const OPchar* target, OPint failIfExists);

//-----------------------------------------------------------------------------
#ifdef __cplusplus
};
#endif
#endif
