#pragma once

struct OPfile;
typedef struct OPfile OPfile;

#include "./Data/include/OPstream.h"
#include "./Data/include/OPfileInformation.h"

// Define platform independent aliases for reading/writing from sockets and files
#define OPFILEREAD(fd, buff, count, readSuccessfully){\
	readSuccessfully = read(fd, buff, count);\
}

#define OPFILEWRITE(fd, buff, count, writtenSuccesfully){\
	writtenSuccesfully = write(fd, buff, count);\
}

struct OPfile {
#ifdef OPIFEX_WINDOWS
	FILE* _handle;
#else
	OPint _handle;
#endif
	const OPchar* path;

	OPfile() { }
	OPfile(const OPchar* path) { Init(path); }

	void Init(const OPchar* path);

	OPstream* Read(ui32 size);
	OPint Writeui8(ui8 data);
	OPint Writeui16(ui16 data);
	OPint Writeui32(ui32 data);
	OPint Writei8(i8 data);
	OPint Writei16(i16 data);
	OPint Writei32(i32 data);
	OPint WriteString(const OPchar* data);
	OPint WriteBytes(void* data, ui64 bytesToWrite);
	ui8 Readui8();
	ui16 Readui16();
	ui32 Readui32();
	i8 Readi8();
	i16 Readi16();
	i32 Readi32();
	OPchar* ReadString();
	void* ReadBytes(ui64 bytesToRead);
	OPint SeekReset();
	OPint Seek(OPint pos);
	OPint Destroy();

	static OPfileInformation Create(const char* path);

	/**
	* OPwriteFile - Writes a byte stream to a file.
	*	@param path Location in the file system where the data will be written.
	*	@param data OPstream instance which will be written
	*	@return 1 on success, 0 on failure
	*/
	static OPint WriteToFile(const char* path, OPstream* data);

	/**
	* OPreadFile - Reads a file into a byte stream.
	*	@param path Location where the data will be read from.
	*	@return pointer to the data stream, NULL on failure
	*/
	static OPstream* ReadFromFile(const char* path);

	/**
	* OPreadFile - Reads a file into a byte stream.
	*	@param path Location where the data will be read from.
	*	@return pointer to the data stream, NULL on failure
	*/
	static OPstream* ReadFromFile(const char* path, ui32 expectedSize);

	/**
	* OPfileExists - Checks for the existance of a file.
	*	@param path Location in the file system where we will look for a file.
	*	@return 1 on success, 0 on failure
	*/
	static bool Exists(const char* path);

	/**
	* OPdeleteFile - Deletes a file from the filesystem.
	*	@param path Location in the file system where we will look for a file.
	*	@return 1 on success, 0 on failure
	*/
	static bool Delete(const char* path);

	static ui64 LastChange(const OPchar* path);

	static OPchar** GetDirectoryFiles(OPchar* path, ui32* count);
};
