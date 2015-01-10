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
	const OPchar* _libraryPathTemp;
#else
	void* _library;
#endif
	const OPchar* _libraryPath;
	ui64 _lastModifiedTime;
	OPlist* _symbols;
} OPsharedLibrary;

EXPORT_METHOD OPsharedLibrary* OPsharedLibraryLoad(const OPchar* path);
EXPORT_METHOD OPint OPsharedLibraryDestroy(OPsharedLibrary* sharedLibrary);
EXPORT_METHOD OPint OPsharedLibraryReload(OPsharedLibrary* sharedLibrary);
EXPORT_METHOD OPsharedLibrarySymbol* OPsharedLibraryLoadSymbol(OPsharedLibrary* sharedLibrary, const OPchar* symbolName);
EXPORT_METHOD OPint OPsharedLibraryClose(OPsharedLibrary* sharedLibrary);

#endif