#include "./../include/OPfile.h"
#include "./Core/include/Log.h"

#ifdef OPIFEX_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
AAssetManager* _mgr;
#endif

FileInformation OPreadFileInformation(const char* path){
	FileInformation file;
#ifdef OPIFEX_ANDROID
	AAsset* asset = AAssetManager_open(_mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		return file;

	off_t _start, _length;
    int fd = AAsset_openFileDescriptor(asset, &_start, &_length);

    FILE* myFile = fdopen(dup(fd), "rb"); 
	if(!myFile){
		OPLog("File not loaded.");
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

void OPfileInit(void* manager){
#ifdef OPIFEX_ANDROID
	_mgr = (AAssetManager*)manager;
#endif
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

//-----------------------------------------------------------------------------
OPstream* OPreadFile(const char* path){
#ifdef OPIFEX_ANDROID
	AAsset* asset = AAssetManager_open(_mgr, path, AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		return 0;	

	off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
	
    FILE* myFile = fdopen(dup(fd), "rb"); 
	fseek(myFile, start, SEEK_SET);

	OPstream* str = OPstreamCreate(length);
	// write the entire file into a stream
	ui8* byte = OPalloc(sizeof(ui8) * length);
	while(fread(&byte, sizeof(ui8), length, myFile)){
		OPwrite(str, &byte, length);
	}
	fclose(myFile); 
	OPseek(str, 0);
	
	return str;

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		printf("%s exists\n", path);

		OPint fd = 0, i;
 
		// be sure that the file could be opened successfully
	 	if(fd = open(path, O_RDONLY)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(4);

			printf("File opened successfully\n");

			// write the entire file into a stream
			while(read(fd, &byte, sizeof(ui8))){
				//char ch = (char)byte;
				//printf("%c", ch);
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
		printf("%s exists\n", path);
 
		// be sure that the file could be opened successfully
	 	if(!_sopen_s(&fd, path, _O_BINARY|_O_RDONLY, _SH_DENYWR, _S_IREAD)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(4);

			printf("File opened successfully\n");

			// write the entire file into a stream
			while(read(fd, &byte, sizeof(ui8))){
				//char ch = (char)byte;
				//printf("%c", ch);
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
			printf("%s exists\n", path);
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
			OPint fd = 0, i;
	 
			// be sure that the file could be opened successfully
		 	if(fd = open(path, O_RDONLY)){
				ui8 byte = 0;
				OPStream* str = new OPStream(sizeof(OPint) * 4);

				printf("File opened successfully\n");

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
