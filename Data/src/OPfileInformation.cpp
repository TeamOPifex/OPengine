#include "./Data/include/OPfileInformation.h"
#include "./Core/include/Assert.h"

void OPfileInformation::Init(const OPchar* path) {
	file = NULL;
	start = 0;
	length = 0;
	fileDescriptor = 0;

#ifdef OPIFEX_ANDROID
	//AAssetManager* mgr = AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager());
	AAsset* asset = AAssetManager_open(OPAndroidState->activity->assetManager, path, AASSET_MODE_UNKNOWN);

	off_t _start, _length;
	int fd = AAsset_openFileDescriptor(asset, &_start, &_length);

	FILE* myFile = fdopen(dup(fd), "rb");
	if (!myFile) {
		OPlog("File not loaded: %s\n", path);
	}
	ASSERT(myFile, "File not loaded");
	fseek(myFile, _start, SEEK_SET);
	file.file = myFile;
	file.start = _start;
	file.length = _length;
	file.fileDescriptor = fd;
#else

#ifdef OPIFEX_WINDOWS
	fopen_s(&file, path, "rb");
#else
	file = fopen(path, "rb");
#endif
	ASSERT(file != NULL, "File not loaded");

	fseek(file, 0, SEEK_END);
	length = (OPuint)ftell(file);
	fseek(file, 0, SEEK_SET);

	fileDescriptor = 0;
	start = 0;
#endif
}