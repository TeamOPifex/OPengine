#ifndef OP_ENGINE_DATA_OPSHAREDLIBRARY
#define OP_ENGINE_DATA_OPSHAREDLIBRARY

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPlist.h"

typedef struct {
	void* Symbol;
	const OPchar* _symbolName;
} OPsharedLibrarySymbol;

typedef struct {
	void* _library;
	const OPchar* _libraryPath;
	OPuint _lastModifiedTime;
	OPlist* _symbols;
} OPsharedLibrary;

OPsharedLibrary* OPsharedLibraryLoad(const OPchar* path);
OPint OPsharedLibraryReload(OPsharedLibrary* sharedLibrary);
OPsharedLibrarySymbol* OPsharedLibraryLoadSymbol(OPsharedLibrary* sharedLibrary, const OPchar* symbolName);
OPint OPsharedLibraryClose(OPsharedLibrary* sharedLibrary);

#endif