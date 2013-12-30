#include "./../include/OPfile.h"
#include "./Core/include/Log.h"

#ifdef OPIFEX_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include "./Core/include/Core.h"
#endif

ui16 OPread_ui16(OPstream* str) {
	OPchar tmp[2];
	OPmemcpy(tmp, OPread(str, sizeof(ui16)), sizeof(ui16));
	return *((ui16*)tmp);
}

ui32 OPread_ui32(OPstream* str) {
	OPchar tmp[4];
	OPmemcpy(tmp, OPread(str, sizeof(ui32)), sizeof(ui32));
	return *((ui32*)tmp);
}

i8 OPread_i8(OPstream* str) {
	OPchar tmp[1];
	OPmemcpy(tmp, OPread(str, sizeof(i8)), sizeof(i8));
	return *((i8*)tmp);
}

i16 OPread_i16(OPstream* str) {
	OPchar tmp[2];
	OPmemcpy(tmp, OPread(str, sizeof(i16)), sizeof(i16));
	return *((i16*)tmp);
}

i32 OPread_i32(OPstream* str) {
	OPchar tmp[4];
	OPmemcpy(tmp, OPread(str, sizeof(i32)), sizeof(i32));
	return *((i32*)tmp);
}

f32 OPread_f32(OPstream* str) {
	OPchar tmp[4];
	OPmemcpy(tmp, OPread(str, sizeof(f32)), sizeof(f32));
	return *((f32*)tmp);
}

FileInformation OPreadFileInformation(const char* path){
	FileInformation file;
#ifdef OPIFEX_ANDROID
	OPLog("OPreadFileInformation: Creating Asset man.\n");
	AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		return file;

	off_t _start, _length;
    int fd = AAsset_openFileDescriptor(asset, &_start, &_length);

    OPLog("OPreadFileInformation(): fd = %d\n", fd);
    FILE* myFile = fdopen(dup(fd), "rb"); 
	if(!myFile){
		OPLog("File not loaded: %s\n", path);
	}
	fseek(myFile, _start, SEEK_SET);
	file.file = myFile;
	file.start = _start;
	file.length = _length;
	file.fileDescriptor = fd;
#else
	FILE* myFile = fopen(path, "rb"); 
	if(!myFile){
		char buff[256];
		sprintf(buff, "OPreadFileInformation(): %s not loaded\n", path);
		OPLog(buff);
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
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	OPint fd = 0;
	
	// be sure that the file could be opened successfully
	if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC)) > 0){
		// write the entire stream in one go.
		write(fd, stream->Data, stream->_pointer);
		// finally close the file, we are done writing
		close(fd);
		return 1;
	}
	else{
		return 0;
	}
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
#endif
}

OPstream* OPreadFile(const char* path) {
	return OPreadFileLarge(path, 1);
}

//-----------------------------------------------------------------------------
OPstream* OPreadFileLarge(const char* path, ui32 expectedCharSize){
#ifdef OPIFEX_ANDROID
	OPLog("OPreadFile: %s\n", path);
	AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL){
		OPLog("OPreadFile: Asset man creation failed.\n");
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

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		printf("OPreadFile: %s\n", path);

		OPint fd = 0, i;
 
		// be sure that the file could be opened successfully
	 	if(fd = open(path, O_RDONLY)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(expectedCharSize * 4);
			
			// write the entire file into a stream
			while(read(fd, &byte, sizeof(ui8))){
				OPwrite(str, &byte, sizeof(ui8));
			}
			close(fd); 
			OPseek(str, 0);

			// finally return the stream
			return str;
		}
	}
	else{
		printf("%s does not exist\n", path);
		return NULL;
	}	
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
	OPint fd = 0, i;
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		printf("OPreadFile: %s\n", path);
 
		// be sure that the file could be opened successfully
	 	if(!_sopen_s(&fd, path, _O_BINARY|_O_RDONLY, _SH_DENYWR, _S_IREAD)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(expectedCharSize * 4);
			
			// write the entire file into a stream
			while(read(fd, &byte, sizeof(ui8))){
				OPwrite(str, &byte, sizeof(ui8));
			}
			close(fd); 
			OPseek(str, 0);

			// finally return the stream
			return str;
		}
	}
	else
		printf("%s does not exist\n", path);
#endif
}

//-----------------------------------------------------------------------------
OPint OPfileExists(const char* path){
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	return access(path, F_OK) + 1;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif
}

//-----------------------------------------------------------------------------
OPint OPdeleteFile(const char* path){
	if(OPfileExists(path)){
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
		return unlink(path) + 1;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif
	}
	else{
		return 0;
	}
}

//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // use the C++ file object
	using namespace OPEngine::Data;

	OPStream* OPFile::Read(const char* path){
		// check to see if the file exists
		if(OPFile::Exists(path) >= 0){
			printf("OPreadFile: %s\n", path);
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
			OPint fd = 0, i;
	 
			// be sure that the file could be opened successfully
		 	if(fd = open(path, O_RDONLY)){
				ui8 byte = 0;
				OPStream* str = new OPStream(sizeof(OPint) * 4);
				
				// write the entire file into a stream
				while(read(fd, &byte, sizeof(ui8)))
					str->Write(&byte, sizeof(ui8));

				close(fd); 
				str->Seek(0);

				// finally return the stream
				return str;
			}
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
#endif
		}
		else{
			printf("%s does not exist\n", path);
			return NULL;
		}
	}

	OPuint OPFile::Write(const char* path, OPStream* stream){
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
		OPint fd = 0;
		OPstream* s = stream->GetStream();	

		// be sure that the file could be opened successfully
		if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, 0666)) >= 0){
			stream->Seek(0);
			// write the entire stream in one go.
			printf("Writing to %d @ %d with %d bytes\n", fd, s->_pointer, s->Length);
			write(fd, s->Data, s->Length);
			// finally close the file, we are done writing
			close(fd); 
			return 1;
		}
		else{
			return 0;
		}
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
		// windows implementation
		OPint fd = 0;
		OPstream* s = stream->GetStream();	

		// be sure that the file could be opened successfully
		if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, 0666)) >= 0){
			stream->Seek(0);
			// write the entire stream in one go.
			printf("Writing to %d @ %d with %d bytes\n", fd, s->_pointer, s->Length);
			write(fd, s->Data, s->Length);
			// finally close the file, we are done writing
			close(fd); 
			return 1;
		}
		else{
			return 0;
		}
	#endif
	}

	OPint OPFile::Exists(const char* path){
		return OPfileExists(path);
	}

	OPint OPFile::Delete(const char* path){
		return OPdeleteFile(path);
	}
#endif
