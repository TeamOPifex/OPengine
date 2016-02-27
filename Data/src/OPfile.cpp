#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(OPIFEX_WINDOWS)
#include <share.h>
#endif

#ifdef OPIFEX_UNIX
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef OPIFEX_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include "./Core/include/OPcore.h"
#endif

ui16 OPreadui16(OPstream* str) {
	OPchar tmp[2];
	OPmemcpy(tmp, OPread(str, sizeof(ui16)), sizeof(ui16));
	return *((ui16*)tmp);
}

ui32 OPreadui32(OPstream* str) {
	OPchar tmp[4];
	OPmemcpy(tmp, OPread(str, sizeof(ui32)), sizeof(ui32));
	return *((ui32*)tmp);
}

OPchar OPreadi8(OPstream* str) {
	OPchar tmp[1];
	OPmemcpy(tmp, OPread(str, sizeof(i8)), sizeof(i8));
	return *((i8*)tmp);
}

i16 OPreadi16(OPstream* str) {
	OPchar tmp[2];
	OPmemcpy(tmp, OPread(str, sizeof(i16)), sizeof(i16));
	return *((i16*)tmp);
}

i32 OPreadi32(OPstream* str) {
	OPchar tmp[4];
	OPmemcpy(tmp, OPread(str, sizeof(i32)), sizeof(i32));
	return *((i32*)tmp);
}

f32 OPreadf32(OPstream* str) {
	OPchar tmp[sizeof(f32)];
	OPmemcpy(tmp, OPread(str, sizeof(f32)), sizeof(f32));
	return *((f32*)tmp);
}

OPchar* OPreadstring(OPstream* str) {
	ui32 j = 0, length = OPreadui32(str);
	OPchar* name = (OPchar*)OPalloc(length + 1);
	for(; j < length; j++){
		name[j] = OPreadi8(str);
	}
	name[length] = '\0';
	return name;
}

OPfileInformation OPreadFileInformation(const char* path){
	OPfileInformation file = {
		NULL, 0, 0, 0
	};

#ifdef OPIFEX_ANDROID
	//AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(OPAndroidState->activity->assetManager, path, AASSET_MODE_UNKNOWN);

	off_t _start, _length;
    int fd = AAsset_openFileDescriptor(asset, &_start, &_length);

    FILE* myFile = fdopen(dup(fd), "rb");
	if(!myFile){
		OPlog("File not loaded: %s\n", path);
	}
    ASSERT(myFile, "File not loaded");
	fseek(myFile, _start, SEEK_SET);
	file.file = myFile;
	file.start = _start;
	file.length = _length;
	file.fileDescriptor = fd;
#else

	FILE* myFile;

	myFile = NULL;
	fopen_s(&myFile, path, "rb");
    ASSERT(myFile != NULL, "File not loaded");
	if(!myFile){
		char buff[256];
		OPlog(buff);
		return file;
	}
	fseek(myFile, 0, SEEK_END );
	file.length = (OPuint)ftell( myFile );
	fseek(myFile, 0, SEEK_SET);
	file.file = myFile;
	file.fileDescriptor = 0;
	file.start = 0;

#endif

	return file;
}

//-----------------------------------------------------------------------------
OPint OPwriteFile(const char* path, OPstream* stream){
#if defined(OPIFEX_UNIX)
	OPint fd = 0;

	// be sure that the file could be opened successfully
	if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC)) > 0){
		// write the entire stream in one go.
		write(fd, stream->Data, stream->_pointer);
		// finally close the file, we are done writing
		close(fd);
		return 1;
	}
	else {
		return 0;
	}
#elif defined(OPIFEX_WINDOWS)
	// windows implementation
	return 0;
#endif
}

OPstream* OPreadFile(const char* path) {
    return OPreadFileLarge(path, 32);
}





//-----------------------------------------------------------------------------
OPstream* OPreadFileLarge(const char* path, ui32 expectedSize){

#ifdef OPIFEX_ANDROID
	AAsset* asset = AAssetManager_open(OPAndroidState->activity->assetManager, path, AASSET_MODE_UNKNOWN);
	if (asset == NULL){
		OPlog("OPreadFile: Asset man creation failed.\n");
		return 0;
	}

	off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);

    FILE* myFile = fdopen(dup(fd), "rb");
	fseek(myFile, start, SEEK_SET);

	OPstream* str = OPstreamCreate(length);

	// write the entire file into a stream
	ui8* byte = OPalloc(sizeof(ui8) * length);
	while(fread(byte, sizeof(ui8), length, myFile)){
		OPwrite(str, byte, length);
	}
	str->Data[length] = 0;

	fclose(myFile);
	OPseek(str, 0);
	return str;

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64) || defined(OPIFEX_IOS)
	ui8 bytes[1024];
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		OPint fd = 0;

		fd = open(path, O_RDONLY);
		// be sure that the file could be opened successfully
	 	if(fd){

			OPstream* str = OPstreamCreate(expectedSize);

			ui8* bytes = (ui8*)OPalloc(1024);
			ui32 readBytes = 1;
			// write the entire file into a stream
			while(readBytes) {
				readBytes = read(fd, bytes, 1024);
				OPwrite(str, bytes, readBytes);
			}
			OPfree(bytes);
			close(fd);
			OPseek(str, 0);

			// finally return the stream
			return str;
		}
	}
	else{
		OPlog("%s does not exist\n", path);
		return NULL;
	}
#elif defined(OPIFEX_WINDOWS)
	// windows implementation
	ui8 bytes[1024];
	i32 fd = 0;
	// check to see if the file exists
	if(OPfileExists(path) > 0) {

		// be sure that the file could be opened successfully
	 	if(!_sopen_s(&fd, path, _O_BINARY|_O_RDONLY, _SH_DENYWR, _S_IREAD)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(expectedSize);

			ui32 readBytes = 1;
			// write the entire file into a stream
			while(readBytes) {
				readBytes = _read(fd, bytes, 1024);
				OPwrite(str, bytes, readBytes);
			}
			_close(fd);
			OPseek(str, 0);

			// finally return the stream
			return str;
		}
		else {
			OPlog("Failed to read file");
			return NULL;
		}
	}
	else {
		OPlog("%s does not exist\n", path);
	}
#endif
		return NULL;
}


//-----------------------------------------------------------------------------
OPstream* OPfileRead(OPfile* path, ui32 size){

#ifdef OPIFEX_ANDROID
	//AAsset* asset = AAssetManager_open(OPAndroidState->activity->assetManager, path, AASSET_MODE_UNKNOWN);
	//
	//if (asset == NULL){
	//	OPlog("OPreadFile: Asset man creation failed.\n");
	//	return 0;
	//}

	//off_t start, length;
	//int fd = AAsset_openFileDescriptor(asset, &start, &length);

	//FILE* myFile = fdopen(dup(fd), "rb");
	//fseek(myFile, start, SEEK_SET);

	FILE* myFile = path->_handle;

	OPstream* str = OPstreamCreate(size);

	// write the entire file into a stream
	ui8* byte = OPalloc(sizeof(ui8)* size);
	while (fread(byte, sizeof(ui8), size, myFile)){
		OPwrite(str, byte, size);
	}
	str->Data[size] = 0;

	OPseek(str, 0);

	str->Source = path->path;
	return str;

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64) || defined(OPIFEX_IOS)
	ui8 bytes[1024];
	ui32 readBytes;

	OPint fd = path->_handle;

	// be sure that the file could be opened successfully
	if (fd){

		OPstream* str = OPstreamCreate(size);

		ui8* bytes = (ui8*)OPalloc(size);
		// write the entire file into a stream
		readBytes = read(fd, bytes, size);
		OPwrite(str, bytes, readBytes);
		OPfree(bytes);
		OPseek(str, 0);

		str->Source = path->path;

		// finally return the stream
		return str;
	}
#elif defined(OPIFEX_WINDOWS)
	// windows implementation
	FILE* fd = path->_handle;
	// be sure that the file could be opened successfully
	ui8 byte = 0;
	OPstream* str = OPstreamCreate(size);
	str->Source = path->path;

	OPwrite(str, OPfileReadBytes(path, size), size);

	OPseek(str, 0);

	// finally return the stream
	return str;
#endif
	return NULL;
}

//-----------------------------------------------------------------------------
OPint OPfileExists(const char* path){
#if defined(OPIFEX_UNIX)
	return access(path, F_OK) + 1;
#elif defined(OPIFEX_WINDOWS)
	FILE *istream = NULL;
	fopen_s(&istream, path, "r");
	if (istream != NULL)
	{
		fclose(istream);
		return 1;
	}
	return 0;
#endif
}

//-----------------------------------------------------------------------------
OPint OPdeleteFile(const char* path){
	if(OPfileExists(path)){
#if defined(OPIFEX_UNIX)
		return unlink(path) + 1;
#elif defined(OPIFEX_WINDOWS)
		return 0;
#endif
	}
	else{
		return 0;
	}
}

ui64 OPfileLastChange(const OPchar* path) {
#ifdef OPIFEX_WINDOWS
	ULONGLONG rtn;
	HANDLE hFile = CreateFile(path, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME ftCreate, ftAccess, ftWrite;
		// Retrieve the file times for the file.
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
			return 0;
		CloseHandle(hFile);
		rtn = (((ULONGLONG)ftWrite.dwHighDateTime) << 32) +
			ftWrite.dwLowDateTime;
		return rtn;
	}
#elif defined(OPIFEX_UNIX)
	struct stat st;
	OPint ierr = stat(path, &st);
	if(ierr != 0) {
		return 0;
	}
	return st.st_mtime;
#endif
	return 0;
}



OPfile OPfileOpen(const OPchar* path) {
#if defined(OPIFEX_UNIX)
	OPint fd = 0;
	OPfile file = { 0 };

	// be sure that the file could be opened successfully
	fd = open(path, O_RDONLY);
	if(fd == 0) return file;

	file._handle = fd;

	return file;

#elif defined(OPIFEX_WINDOWS)
	// windows implementation

	OPfile file = { NULL };
	if (OPfileExists(path) > 0) {
		fopen_s(&file._handle, path, "r+b");
	}
	file.path = path;
	return file;

#endif
}

OPint OPfileWriteui8(OPfile* file, ui8 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui8), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(ui8));
#endif
    return 1;
}
OPint OPfileWriteui16(OPfile* file, ui16 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui16), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(ui16));
#endif
    return 1;
}
OPint OPfileWriteui32(OPfile* file, ui32 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui32), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(ui32));
#endif
    return 1;
}
OPint OPfileWritei8(OPfile* file, i8 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i8), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(i8));
#endif
    return 1;
}
OPint OPfileWritei16(OPfile* file, i16 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i16), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(i16));
#endif
    return 1;
}
OPint OPfileWritei32(OPfile* file, i32 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i32), 1, file->_handle);
#else
	write((int)file->_handle, &data, sizeof(i32));
#endif
    return 1;
}
OPint OPfileWriteString(OPfile* file, const OPchar* data) {
	OPuint len = (OPuint)strlen(data);
#ifdef OPIFEX_WINDOWS
	fwrite((void*)len, sizeof(ui32), 1, file->_handle);
	fwrite(data, sizeof(OPchar), len, file->_handle);
#else
	write((int)file->_handle, (void*)len, sizeof(ui32));
	write((int)file->_handle, data, sizeof(OPchar)* len);
#endif
    return 1;
}

OPint OPfileWriteBytes(OPfile* file, void* data, ui64 bytesToWrite) {
#ifdef OPIFEX_WINDOWS
	return fwrite(data, sizeof(char), bytesToWrite, file->_handle);
#else
	return (OPint)write(file->_handle, data, bytesToWrite);
#endif
}

ui8 OPfileReadui8(OPfile* file) {
	i8 bytes[sizeof(ui8)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui8), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(ui8));
#endif
	return *((ui8*)bytes);
}

ui16 OPfileReadui16(OPfile* file) {
	i8 bytes[sizeof(ui16)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui16), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(ui16));
#endif
	return *((ui16*)bytes);
}

ui32 OPfileReadui32(OPfile* file) {
	i8 bytes[sizeof(ui32)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui32), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(ui32));
#endif
	return *((ui32*)bytes);
}

i8 OPfileReadi8(OPfile* file) {
	i8 bytes[sizeof(i8)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i8), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(i8));
#endif
	return *((i8*)bytes);
}

i16 OPfileReadi16(OPfile* file) {
	i8 bytes[sizeof(i16)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i16), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(i16));
#endif
	return *((i16*)bytes);
}

i32 OPfileReadi32(OPfile* file) {
	i8 bytes[sizeof(i32)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i32), 1, file->_handle);
#else
	read((int)file->_handle, bytes, sizeof(i32));
#endif
	return *((i32*)bytes);
}

OPchar* OPfileReadString(OPfile* file) {
	ui32 len = OPfileReadui32(file);
	OPchar* str = (OPchar*)OPalloc(sizeof(OPchar)* (len + 1));
#ifdef OPIFEX_WINDOWS
	fread(str, sizeof(OPchar), len, file->_handle);
#else
	read((int)file->_handle, str, sizeof(OPchar)* len);
#endif
	str[len] = NULL;
	return str;
}

void* OPfileReadBytes(OPfile* file, ui64 bytesToRead) {
	void* bytes = OPalloc(bytesToRead);
#ifdef OPIFEX_WINDOWS
	OPint bytesRead = fread(bytes, sizeof(OPchar), bytesToRead, file->_handle);
#else
	OPint bytesRead = read(file->_handle, bytes, bytesToRead);
#endif
	if(bytesRead != bytesToRead) return NULL;
	return bytes;
}

OPint OPfileSeek(OPfile* file, OPint pos) {
#ifdef OPIFEX_WINDOWS
	fseek(file->_handle, (long)pos, SEEK_SET);
#else
    lseek(file->_handle, pos, SEEK_SET);
#endif
    return 1;
}

OPint OPfileSeekReset(OPfile* file) {
#ifdef OPIFEX_WINDOWS
	OPfileSeek(file, 0);
#else
	lseek(file->_handle, 0, SEEK_SET);
#endif
	return 1;
}

OPint OPfileClose(OPfile* file) {
#ifdef OPIFEX_WINDOWS
	fclose(file->_handle);
#else
    close(file->_handle);
#endif
    return 1;
}


OPfileInformation OPfileCreate(const char* path) {
	OPfileInformation result;

	result.start = 0;
	result.length = 0;
	fopen_s(&result.file, path, "w+");

	_sopen_s(&result.fileDescriptor, path, O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);

	return result;
}