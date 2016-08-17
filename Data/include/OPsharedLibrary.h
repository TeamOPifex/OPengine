#pragma once

struct OPsharedLibrary;
typedef struct OPsharedLibrary OPsharedLibrary;

#include "./Data/include/OPsharedLibrarySymbol.h"
#include "./Data/include/OPlist.h"

#ifdef OPIFEX_WINDOWS
	#define OPEXPORT extern "C" __declspec(dllexport)
#else
	#define OPEXPORT extern "C"
#endif

struct OPsharedLibrary {
#ifdef OPIFEX_WINDOWS
	HMODULE _library;
#else
	void* _library;
#endif
	const OPchar* _libraryPath;
	OPuint _lastModifiedTime;
	OPlist* _symbols;

	OPsharedLibrary() { }
	OPsharedLibrary(const OPchar* path) { Init(path); }

	void Init(const OPchar* path);
	bool Reload();
	OPsharedLibrarySymbol* LoadSymbol(const OPchar* symbolName);
	void Destroy();
};