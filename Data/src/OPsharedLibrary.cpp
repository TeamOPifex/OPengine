#include "./Data/include/OPsharedLibrary.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPcore.h"
#include "./Core/include/Assert.h"

// TODO: abstract out to Windows and Linux
#ifdef OPIFEX_UNIX
	#include <dlfcn.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif

void OPsharedLibrary::Init(const OPchar* libraryName) {

#ifdef OPIFEX_UNIX
	OPchar* temp = OPstringCreateMerged("lib", libraryName);
	OPchar* lib = OPstringCreateMerged(temp, ".dylib");
	OPchar* path = OPstringCreateMerged(OPgetExecutableDir(), lib);
	OPlog(path);
	void* library = dlopen(path, RTLD_NOW);
	if(library == NULL) return NULL;
#elif defined(OPIFEX_WINDOWS)
	OPchar* lib = OPstringCreateMerged(libraryName, ".dll");
	OPchar* path = OPstringCreateMerged(OPEXECUTABLE_PATH, lib);
	HMODULE library = LoadLibraryA(path);
	ASSERT(library != NULL, "FAILED to load library");
#endif

	_library = library;
	_libraryPath = path;
	_symbols = OPlist::Create(4, sizeof(OPsharedLibrarySymbol));
	_lastModifiedTime = OPfile::LastChange(_libraryPath);
}

bool OPsharedLibrary::Reload() {
	ui64 lastChange = OPfile::LastChange(_libraryPath);
	if (_lastModifiedTime == lastChange) return false;

	_lastModifiedTime = lastChange;
	OPlog("Last Change: %d", lastChange);

	Destroy();
#ifdef OPIFEX_UNIX

	void* library = dlopen(_libraryPath, RTLD_NOW);
	if(library == NULL) return -2;

	_library = library;

	//return 1;

	OPint elements = OPlistSize(_symbols);
	void* symbol;
	OPint result = 0;
	for(OPint i = 0; i < elements; i++) {
		OPlog("Get %d", i);

		OPsharedLibrarySymbol* sharedLibrarySymbol = (OPsharedLibrarySymbol*)OPlistGet(sharedLibrary->_symbols, i);
		symbol = dlsym(sharedLibrary->_library, sharedLibrarySymbol->_symbolName);
		if(symbol == NULL) {
			OPlog("!!!   Failed to reload symbol: %s", sharedLibrarySymbol->_symbolName);
			result = -3;
		} else {
			OPlog("Symbol Reloaded: %s", sharedLibrarySymbol->_symbolName);
		}
		sharedLibrarySymbol->Symbol = symbol;
		OPlog("Symbol set");
	}

	OPlog("Finished setting");

	return result;
#elif defined(OPIFEX_WINDOWS)
	HMODULE library = LoadLibraryA(_libraryPath);
	if (library == NULL) return false;
	_library = library;

	OPint elements = _symbols->Size();
	void* symbol;
	OPint result = 0;
	for (OPint i = 0; i < elements; i++) {
		OPsharedLibrarySymbol* sharedLibrarySymbol = (OPsharedLibrarySymbol*)_symbols->Get(i);
		symbol = (void*)GetProcAddress(_library, sharedLibrarySymbol->_symbolName);
		if (symbol == NULL) {
			OPlogErr("!!!   Failed to reload symbol: %s", sharedLibrarySymbol->_symbolName);
			return false;
		}
		sharedLibrarySymbol->Symbol = symbol;
	}

	return true;
#endif
}

OPsharedLibrarySymbol* OPsharedLibrary::LoadSymbol(const OPchar* symbolName) {
#ifdef OPIFEX_UNIX
	void* symbol = dlsym(sharedLibrary->_library, symbolName);
#elif defined(OPIFEX_WINDOWS)
	void* symbol = (void*)GetProcAddress(_library, symbolName);
#endif
	if (symbol == NULL) return NULL;

	OPsharedLibrarySymbol sharedLibrarySymbol = {
		symbol,
		symbolName
	};
	_symbols->Push((ui8*)&sharedLibrarySymbol);
	return (OPsharedLibrarySymbol*)_symbols->Peek();
}

void OPsharedLibrary::Destroy() {
#ifdef OPIFEX_UNIX
	dlclose(_library);
#elif defined(OPIFEX_WINDOWS)
	FreeLibrary(_library);
#endif
	_symbols->Destroy();
	OPfree(_symbols);
}

