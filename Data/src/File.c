#include "./../include/File.h"
#include "./Core/include/Log.h"
#include "./Core/include/Assert.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(OPIFEX_WINDOWS)
#include <share.h>
#endif

#ifdef OPIFEX_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include "./Core/include/Core.h"
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

i8 OPreadi8(OPstream* str) {
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

i8* OPreadstring(OPstream* str) {
	ui32 length = OPreadui32(str);
	i8* name = (i8*)OPalloc(length + 1);
	for(i32 j = 0; j < length; j++){
		name[j] = OPreadi8(str);
	}
	name[length] = NULL;
	return name;
}

OPfileInformation OPreadFileInformation(const char* path){
	OPfileInformation file;

#ifdef OPIFEX_ANDROID
	AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		return file;

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
	FILE* myFile = fopen(path, "rb"); 
    ASSERTC(myFile, "File not loaded");
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
	AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL){
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
		OPlog("OPreadFile: %s\n", path);

		OPint fd = 0, i;
 
		// be sure that the file could be opened successfully
	 	if(fd = open(path, O_RDONLY)){

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
	if(OPfileExists(path) >= 0) {
		OPlog("OPreadFile: %s\n", path);
 
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
	}
	else
		OPlog("%s does not exist\n", path);
#endif
}

//-----------------------------------------------------------------------------
OPint OPfileExists(const char* path){
#if defined(OPIFEX_UNIX)
	return access(path, F_OK) + 1;
#elif defined(OPIFEX_WINDOWS)

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
