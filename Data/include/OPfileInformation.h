#pragma once

struct OPfileInformation;
typedef struct OPfileInformation OPfileInformation;

#include "./Data/include/OPstream.h"
#include <stdio.h>
#if defined(OPIFEX_UNIX)
#include <unistd.h>
#elif defined(OPIFEX_WINDOWS)
#include <io.h>
#endif

/**
* OPreadFileInformation - Reads file information, getting start and length
*/
struct OPfileInformation {
	FILE* file;
	ui32 start;
	ui32 length;
	int fileDescriptor;

	OPfileInformation() { }
	OPfileInformation(const OPchar* path) { Init(path); }

	void Init(const OPchar* path);
};
