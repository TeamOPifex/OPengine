#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"
#include "./Data/include/OPstring.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(OPIFEX_WINDOWS)
#include <share.h>
#include <stdio.h>
#include <fstream>
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



OPint OPfile::WriteToFile(const char* path, OPstream* stream){
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

OPstream* OPfile::ReadFromFile(const char* path) {
    return OPfile::ReadFromFile(path, 2048);
}

OPstream* OPfile::ReadFromFile(const char* path, ui32 expectedSize){

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
	str->Source = OPstringCopy(path);

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
	if(OPfile::Exists(path)){
		OPint fd = 0;

		fd = open(path, O_RDONLY);
		// be sure that the file could be opened successfully
	 	if(fd){

			OPstream* str = OPstream::Create(expectedSize);
			str->Source = OPstringCopy(path);

			ui8* bytes = (ui8*)OPalloc(1024);
			ui32 readBytes = 1;
			// write the entire file into a stream
			while(readBytes) {
				readBytes = read(fd, bytes, 1024);
				str->Write(bytes, readBytes);
			}
			OPfree(bytes);
			close(fd);
            str->Seek(0);

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
	if(OPfile::Exists(path)) {

		// be sure that the file could be opened successfully
	 	if(!_sopen_s(&fd, path, _O_BINARY|_O_RDONLY, _SH_DENYWR, _S_IREAD)){
			ui8 byte = 0;
			OPstream* str = OPstream::Create(expectedSize);
			str->Source = OPstringCopy(path);

			ui32 readBytes = 1;
			// write the entire file into a stream
			while(readBytes) {
				readBytes = _read(fd, bytes, 1024);
				str->Write(bytes, readBytes);
			}
			_close(fd);
			str->Seek(0);

			// finally return the stream
			return str;
		}
		else {
			OPlogErr("Failed to read file");
			return NULL;
		}
	}
	else {
		OPlogErr("%s does not exist", path);
	}
#endif
		return NULL;
}


OPstream* OPfile::Read(ui32 size){

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

	FILE* myFile = _handle;

	OPstream* str = OPstream::Create(size);

	// write the entire file into a stream
	ui8* byte = OPalloc(sizeof(ui8)* size);
	while (fread(byte, sizeof(ui8), size, myFile)){
		OPwrite(str, byte, size);
	}
	str->Data[size] = 0;

	OPseek(str, 0);

	str->Source = OPstringCopy(path);
	return str;

#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64) || defined(OPIFEX_IOS)
	ui8 bytes[1024];
	ui32 readBytes;

	OPint fd = _handle;

	// be sure that the file could be opened successfully
	if (fd){

		OPstream* str = OPstream::Create(size);

		ui8* bytes = (ui8*)OPalloc(size);
		// write the entire file into a stream
		readBytes = read(fd, bytes, size);
        str->Write(bytes, readBytes);
		OPfree(bytes);
        str->Seek(0);

		str->Source = OPstringCopy(path);

		// finally return the stream
		return str;
	}
#elif defined(OPIFEX_WINDOWS)
	// windows implementation
	FILE* fd = _handle;
	// be sure that the file could be opened successfully
	ui8 byte = 0;

	OPstream* str = OPstream::Create(size);
	str->Source = OPstringCopy(path);
	str->Write(ReadBytes(size), size);
	str->Seek(0);

	// finally return the stream
	return str;
#endif
	return NULL;
}

//-----------------------------------------------------------------------------
bool OPfile::Exists(const char* path){
#if defined(OPIFEX_UNIX)
	return access(path, F_OK) + 1;
#elif defined(OPIFEX_WINDOWS)
	if (std::ifstream(path)) {
		return true;
	}
	return false;
	//FILE *istream = NULL;
	//fopen_s(&istream, path, "r");
	//if (istream != NULL)
	//{
	//	fclose(istream);
	//	return true;
	//}
	//return false;
#endif
}

//-----------------------------------------------------------------------------
bool OPfile::Delete(const char* path){
	if(OPfile::Exists(path)){
#if defined(OPIFEX_UNIX)
		return (unlink(path) + 1) == 1;
#elif defined(OPIFEX_WINDOWS)
		return false;
#endif
	}
	return false;
}

ui64 OPfile::LastChange(const OPchar* path) {
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



void OPfile::Init(const OPchar* path) {
#if defined(OPIFEX_UNIX)
	OPint fd = 0;
	// be sure that the file could be opened successfully
	fd = open(path, O_RDONLY);
    ASSERT(fd != 0, "Failed to open file");

	_handle = fd;
	this->path = path;
#elif defined(OPIFEX_WINDOWS)
	// windows implementation
	if (OPfile::Exists(path)) {
		fopen_s(&_handle, path, "r+b");
	}
	this->path = path;
#endif
}

OPint OPfile::Writeui8(ui8 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui8), 1, _handle);
#else
	write((int)_handle, &data, sizeof(ui8));
#endif
    return 1;
}
OPint OPfile::Writeui16(ui16 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui16), 1, _handle);
#else
	write((int)_handle, &data, sizeof(ui16));
#endif
    return 1;
}
OPint OPfile::Writeui32(ui32 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(ui32), 1, _handle);
#else
	write((int)_handle, &data, sizeof(ui32));
#endif
    return 1;
}
OPint OPfile::Writei8(i8 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i8), 1, _handle);
#else
	write((int)_handle, &data, sizeof(i8));
#endif
    return 1;
}
OPint OPfile::Writei16(i16 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i16), 1, _handle);
#else
	write((int)_handle, &data, sizeof(i16));
#endif
    return 1;
}
OPint OPfile::Writei32(i32 data) {
#ifdef OPIFEX_WINDOWS
	fwrite(&data, sizeof(i32), 1, _handle);
#else
	write((int)_handle, &data, sizeof(i32));
#endif
    return 1;
}
OPint OPfile::WriteString(const OPchar* data) {
	OPuint len = (OPuint)strlen(data);
#ifdef OPIFEX_WINDOWS
	fwrite((void*)len, sizeof(ui32), 1, _handle);
	fwrite(data, sizeof(OPchar), len, _handle);
#else
	write((int)_handle, (void*)len, sizeof(ui32));
	write((int)_handle, data, sizeof(OPchar)* len);
#endif
    return 1;
}

OPint OPfile::WriteBytes(void* data, ui64 bytesToWrite) {
#ifdef OPIFEX_WINDOWS
	return fwrite(data, sizeof(char), bytesToWrite, _handle);
#else
	return (OPint)write(_handle, data, bytesToWrite);
#endif
}

ui8 OPfile::Readui8() {
	i8 bytes[sizeof(ui8)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui8), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(ui8));
#endif
	return *((ui8*)bytes);
}

ui16 OPfile::Readui16() {
	i8 bytes[sizeof(ui16)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui16), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(ui16));
#endif
	return *((ui16*)bytes);
}

ui32 OPfile::Readui32() {
	i8 bytes[sizeof(ui32)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(ui32), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(ui32));
#endif
	return *((ui32*)bytes);
}

i8 OPfile::Readi8() {
	i8 bytes[sizeof(i8)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i8), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(i8));
#endif
	return *((i8*)bytes);
}

i16 OPfile::Readi16() {
	i8 bytes[sizeof(i16)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i16), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(i16));
#endif
	return *((i16*)bytes);
}

i32 OPfile::Readi32() {
	i8 bytes[sizeof(i32)];
#ifdef OPIFEX_WINDOWS
	fread(bytes, sizeof(i32), 1, _handle);
#else
	read((int)_handle, bytes, sizeof(i32));
#endif
	return *((i32*)bytes);
}

OPchar* OPfile::ReadString() {
	ui32 len = Readui32();
	OPchar* str = (OPchar*)OPalloc(sizeof(OPchar)* (len + 1));
#ifdef OPIFEX_WINDOWS
	fread(str, sizeof(OPchar), len, _handle);
#else
	read((int)_handle, str, sizeof(OPchar)* len);
#endif
	str[len] = NULL;
	return str;
}

void* OPfile::ReadBytes(ui64 bytesToRead) {
	void* bytes = OPalloc(bytesToRead);
#ifdef OPIFEX_WINDOWS
	OPint bytesRead = fread(bytes, sizeof(OPchar), bytesToRead, _handle);
#else
	OPint bytesRead = read(_handle, bytes, bytesToRead);
#endif
	if(bytesRead != bytesToRead) return NULL;
	return bytes;
}

OPint OPfile::Seek(OPint pos) {
#ifdef OPIFEX_WINDOWS
	fseek(_handle, (long)pos, SEEK_SET);
#else
    lseek(_handle, pos, SEEK_SET);
#endif
    return 1;
}

OPint OPfile::SeekReset() {
#ifdef OPIFEX_WINDOWS
	Seek(0);
#else
	lseek(_handle, 0, SEEK_SET);
#endif
	return 1;
}

OPint OPfile::Destroy() {
#ifdef OPIFEX_WINDOWS
	fclose(_handle);
#else
    close(_handle);
#endif
    return 1;
}


OPfileInformation OPfile::Create(const char* path) {
	OPfileInformation result;

	result.start = 0;
	result.length = 0;
	#ifdef OPIFEX_WINDOWS
	fopen_s(&result.file, path, "w+");
	#else
	result.file = fopen(path, "w+");
	#endif

    #ifdef OPIFEX_WINDOWS
	_sopen_s(&result.fileDescriptor, path, O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	#else
	result.fileDescriptor = open(path, O_RDWR);
	#endif

	return result;
}

#ifdef OPIFEX_WINDOWS
#include <windows.h>

ui32 OPdirFilesCount(const OPchar* dir) {
	ui32 total = 0;

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(dir, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				total++;
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return total;
}

OPchar** OPfile::GetDirectoryFiles(OPchar* path, ui32* count) {
	OPstring dirPath(path);
	dirPath.Add("*");
	//if (dirPath.EndsWith("\\")) {
	//	dirPath._data[dirPath._len - 1] = NULL;
	//}

	ui32 total = OPdirFilesCount(dirPath.C_Str());
	OPchar** result = OPALLOC(OPchar*, total);

	*count = total;

	ui32 index = 0;

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(dirPath.C_Str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				result[index++] = OPstringCopy(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return result;
}
#else

ui32 OPdirFilesCount(const OPchar* dir) {
	return 0;
}

OPchar** OPfile::GetDirectoryFiles(OPchar* path, ui32* count) {
	(*count) = 0;
	return NULL;
}
#endif
