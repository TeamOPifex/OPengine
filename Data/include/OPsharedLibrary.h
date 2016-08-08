#ifndef OP_ENGINE_DATA_OPSHAREDLIBRARY
#define OP_ENGINE_DATA_OPSHAREDLIBRARY

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPlist.h"
#ifdef OPIFEX_WINDOWS
#define OPEXPORT extern "C" __declspec(dllexport)
#else
#define OPEXPORT extern "C"
#endif

struct OPsharedLibrarySymbol {
	void* Symbol;
	const OPchar* _symbolName;
};
typedef struct OPsharedLibrarySymbol OPsharedLibrarySymbol;

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


#define OPDLL_LIB lib
#define OPDLL_DYLIB .dylib

#define STRINGIFY_NX(A) #A
#define STRINGIFY(A) STRINGIFY_NX(A)
#define STRING_MERGE_3_NX(A,B,C) A ## B ## C
#define STRING_MERGE_3(A,B,C) STRING_MERGE_NX(A,B,C)
#define OPDLL_PATH(A) "lib" #A ".dylib"

struct OPdll;
typedef struct OPdll OPdll;

struct OPdll {
	#ifdef OPIFEX_WINDOWS
		HMODULE library;
	#else
		void* library;
	#endif
	const OPchar* path;
	#ifdef _DEBUG
	OPuint lastModified;
	#endif
};
OPdll OPdllOpen(const OPchar* path);
OPint OPdllUpdate(OPdll* dll);
void* OPdllSymbol(OPdll* dll, const OPchar* symbol);
void OPdllClose(OPdll* dll);

#endif
