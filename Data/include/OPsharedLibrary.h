#ifndef OP_ENGINE_DATA_OPSHAREDLIBRARY
#define OP_ENGINE_DATA_OPSHAREDLIBRARY

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPlist.h"
#ifdef OPIFEX_WINDOWS
	#include <windows.h>
#define OPEXPORT extern "C" __declspec(dllexport)
#else
#define OPEXPORT extern "C"
#endif

typedef struct {
	void* Symbol;
	const OPchar* _symbolName;
} OPsharedLibrarySymbol;

typedef struct {
#ifdef OPIFEX_WINDOWS
	HMODULE _library;
#else
	void* _library;
#endif
	const OPchar* _libraryPath;
	OPuint _lastModifiedTime;
	OPlist* _symbols;
} OPsharedLibrary;

OPsharedLibrary* OPsharedLibraryLoad(const OPchar* path);
OPint OPsharedLibraryDestroy(OPsharedLibrary* sharedLibrary);
OPint OPsharedLibraryReload(OPsharedLibrary* sharedLibrary);
OPsharedLibrarySymbol* OPsharedLibraryLoadSymbol(OPsharedLibrary* sharedLibrary, const OPchar* symbolName);
OPint OPsharedLibraryClose(OPsharedLibrary* sharedLibrary);

#endif