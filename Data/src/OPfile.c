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
	OPfileInformation file;

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
	myFile = fopen(path, "rb");
    ASSERT(myFile != NULL, "File not loaded");
	if(!myFile){
		char buff[256];
		OPlog(buff);
		return file;
	}
	fseek(myFile, 0, SEEK_END );	
	file.length = ftell( myFile );
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
#endif
}

OPstream* OPreadFile(const char* path) {
	return OPreadFileLarge(path, 32);
}





//-----------------------------------------------------------------------------
OPstream* OPreadFileLarge(const char* path, ui32 expectedSize){
#ifdef OPIFEX_ANDROID
	OPlog("OPreadFile: %s\n", path);
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

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		OPint fd = 0;

		OPlog("OPreadFile: %s", path);
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
	OPint fd = 0, i;
	// check to see if the file exists
	if(OPfileExists(path) > 0) {
		OPlog("OPreadFile: %s", path);
 
		// be sure that the file could be opened successfully
	 	if(!_sopen_s(&fd, path, _O_BINARY|_O_RDONLY, _SH_DENYWR, _S_IREAD)){
			ui8 byte = 0;
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
OPint OPfileExists(const char* path){
#if defined(OPIFEX_UNIX)
	return access(path, F_OK) + 1;
#elif defined(OPIFEX_WINDOWS)
	FILE *istream;
	if ((istream = fopen(path, "r")) != NULL)
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
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC);
	if(fd == 0) return file;

	file._handle = fd;

	return file;

#elif defined(OPIFEX_WINDOWS)
	// windows implementation

	FILE* myFile = fopen(path, "w+b");
	OPfile file = { myFile };
	return file;

#endif
}

OPint OPfileWriteui8(OPfile* file, ui8 data) {
	write(file->_handle, &data, sizeof(ui8));
    return 1;
}
OPint OPfileWriteui16(OPfile* file, ui16 data) {
    write(file->_handle, &data, sizeof(ui16));
    return 1;
}
OPint OPfileWriteui32(OPfile* file, ui32 data) {
    write(file->_handle, &data, sizeof(ui32));
    return 1;
}
OPint OPfileWritei8(OPfile* file, i8 data) {
    write(file->_handle, &data, sizeof(i8));
    return 1;
}
OPint OPfileWritei16(OPfile* file, i16 data) {
    write(file->_handle, &data, sizeof(i16));
    return 1;
}
OPint OPfileWritei32(OPfile* file, i32 data) {
    write(file->_handle, &data, sizeof(i32));
    return 1;
}
OPint OPfileWriteString(OPfile* file, const OPchar* data) {
	ui32 len = strlen(data);
	write(file->_handle, len, sizeof(ui32));
    write(file->_handle, data, sizeof(OPchar) * len);
    return 1;
}

OPint OPfileWriteBytes(OPfile* file, void* data, ui64 bytesToWrite) {
#ifdef OPIFEX_WINDOWS
	return fwrite(data, sizeof(char), bytesToWrite, file->_handle);
#else
	return write(file->_handle, data, bytesToWrite);
#endif
}

ui8 OPfileReadui8(OPfile* file) {
	i8 bytes[sizeof(ui8)];
	read(file->_handle, bytes, sizeof(ui8));
	return *((ui8*)bytes);
}

ui16 OPfileReadui16(OPfile* file) {
	i8 bytes[sizeof(ui16)];
	read(file->_handle, bytes, sizeof(ui16));
	return *((ui16*)bytes);
}

ui32 OPfileReadui32(OPfile* file) {
	i8 bytes[sizeof(ui32)];
	read(file->_handle, bytes, sizeof(ui32));
	return *((ui32*)bytes);
}

i8 OPfileReadi8(OPfile* file) {
	i8 bytes[sizeof(i8)];
	read(file->_handle, bytes, sizeof(i8));
	return *((i8*)bytes);
}

i16 OPfileReadi16(OPfile* file) {
	i8 bytes[sizeof(i16)];
	read(file->_handle, bytes, sizeof(i16));
	return *((i16*)bytes);
}

i32 OPfileReadi32(OPfile* file) {
	i8 bytes[sizeof(i32)];
	read(file->_handle, bytes, sizeof(i32));
	return *((i32*)bytes);
}

OPchar* OPfileReadString(OPfile* file) {
	ui32 len = OPfileReadui32(file);
	OPchar* str = (OPchar*)OPalloc(sizeof(OPchar) * len);
	read(file->_handle, str, sizeof(OPchar) * len);
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
	fseek(file->_handle, 0, SEEK_SET);
#else
    lseek(file->_handle, pos, SEEK_SET);
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