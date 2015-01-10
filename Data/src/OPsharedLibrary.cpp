#include "./Data/include/OPsharedLibrary.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPcore.h"

// TODO: abstract out to Windows and Linux
#ifdef OPIFEX_UNIX
	#include <dlfcn.h>
	#include <sys/stat.h> 
	#include <unistd.h>
#endif

OPsharedLibrary* OPsharedLibraryLoad(const OPchar* libraryName) {

#ifdef OPIFEX_UNIX
	OPchar* temp = OPstringCreateMerged("lib", libraryName);
	OPchar* lib = OPstringCreateMerged(temp, ".dylib");
	OPchar* path = OPstringCreateMerged(OPgetExecutableDir(), lib);
	OPlog(path);
	void* library = dlopen(path, RTLD_NOW);
	if(library == NULL) return NULL;
#elif defined(OPIFEX_WINDOWS)
	OPchar* lib = OPstringCreateMerged(libraryName, ".dll");
	OPchar* libTemp = OPstringCreateMerged(libraryName, ".temp.dll");
	OPchar* path = OPstringCreateMerged(OPgetExecutableDir(), lib);
	OPchar* pathTemp = OPstringCreateMerged(OPgetExecutableDir(), libTemp);
	OPfileCopy(path, pathTemp, false);
	OPlog(path);
	OPlog(pathTemp);
	HMODULE library = LoadLibraryA(pathTemp);
	if (library == NULL) return NULL;
#endif

	OPsharedLibrary* sharedLibrary = (OPsharedLibrary*)OPalloc(sizeof(OPsharedLibrary));
	sharedLibrary->_library = library;
	sharedLibrary->_libraryPath = path;
#ifdef OPIFEX_WINDOWS
	sharedLibrary->_libraryPathTemp = pathTemp;
#endif
	sharedLibrary->_symbols = OPlistCreate(4, sizeof(OPsharedLibrarySymbol));
	sharedLibrary->_lastModifiedTime = OPfileLastChange(sharedLibrary->_libraryPath);

	return sharedLibrary;
}

OPint OPsharedLibraryDestroy(OPsharedLibrary* sharedLibrary) {
	OPlistDestroy(sharedLibrary->_symbols);
	OPfree(sharedLibrary);
	return true;
}

OPint OPsharedLibraryReload(OPsharedLibrary* sharedLibrary) {
	ui64 lastChange = OPfileLastChange(sharedLibrary->_libraryPath);

	if (sharedLibrary->_lastModifiedTime == lastChange) return 0;

	OPlog("Reloading %s, %s", sharedLibrary->_libraryPath, sharedLibrary->_libraryPathTemp);
	OPlog("Last Change: %llu Was: %llu", lastChange, sharedLibrary->_lastModifiedTime);
	sharedLibrary->_lastModifiedTime = lastChange;

	if(OPsharedLibraryClose(sharedLibrary) != 0) {
		OPlog("Failed to close library.");
		return -1;
	}

#ifdef OPIFEX_UNIX

	void* library = dlopen(sharedLibrary->_libraryPath, RTLD_NOW);
	if(library == NULL) return -2;

	sharedLibrary->_library = library;

	OPint elements = OPlistSize(sharedLibrary->_symbols);
	void* symbol;
	OPint result = 0;
	for(OPint i = 0; i < elements; i++) {
		OPsharedLibrarySymbol* sharedLibrarySymbol = (OPsharedLibrarySymbol*)OPlistGet(sharedLibrary->_symbols, i);
		symbol = dlsym(sharedLibrary->_library, sharedLibrarySymbol->_symbolName);
		if(symbol == NULL) {
			OPlog("!!!   Failed to reload symbol: %s", sharedLibrarySymbol->_symbolName);
			result = -3;
		} else {
			OPlog("Ssymbol Reloaded: %s", sharedLibrarySymbol->_symbolName);
		}
		sharedLibrarySymbol->Symbol = symbol;
	}

	return result;

#elif defined(OPIFEX_WINDOWS)
	OPfileCopy(sharedLibrary->_libraryPath, sharedLibrary->_libraryPathTemp, false);
	HMODULE library = NULL;
	while (library == NULL) {
		library = LoadLibraryA(sharedLibrary->_libraryPathTemp);
		if (library == NULL) {
			OPlog("Failed to reload library");
			return -2;
		}
		OPlog("Library reloaded");
	}
	sharedLibrary->_library = library;

	OPint elements = OPlistSize(sharedLibrary->_symbols);
	OPlog("Symbols %d", elements);
	void* symbol;
	OPint result = 0;
	for (OPint i = 0; i < elements; i++) {
		OPsharedLibrarySymbol* sharedLibrarySymbol = (OPsharedLibrarySymbol*)OPlistGet(sharedLibrary->_symbols, i);
		symbol = (void*)GetProcAddress(sharedLibrary->_library, sharedLibrarySymbol->_symbolName);
		if (symbol == NULL) {
			OPlog("!!!   Failed to reload symbol: %s", sharedLibrarySymbol->_symbolName);
			result = -3;
		}
		else {
			OPlog("Symbol Reloaded %s", sharedLibrarySymbol->_symbolName);
		}
		sharedLibrarySymbol->Symbol = symbol;
	}

	return result;

#endif
}

OPsharedLibrarySymbol* OPsharedLibraryLoadSymbol(OPsharedLibrary* sharedLibrary, const OPchar* symbolName) {
#ifdef OPIFEX_UNIX
	void* symbol = dlsym(sharedLibrary->_library, symbolName);
#elif defined(OPIFEX_WINDOWS)
	void* symbol = (void*)GetProcAddress(sharedLibrary->_library, symbolName);
#endif
	if (symbol == NULL) return NULL;

	OPsharedLibrarySymbol sharedLibrarySymbol = {
		symbol,
		symbolName
	};
	OPlistPush(sharedLibrary->_symbols, (ui8*)&sharedLibrarySymbol);
	return (OPsharedLibrarySymbol*)OPlistPeek(sharedLibrary->_symbols);
}

OPint OPsharedLibraryClose(OPsharedLibrary* sharedLibrary) {
#ifdef OPIFEX_UNIX
	OPint result = dlclose(sharedLibrary->_library);
	if (result != 0) return 1;
#elif defined(OPIFEX_WINDOWS)
	OPint result = FreeLibrary(sharedLibrary->_library);
	if (result != 0) return 0;
#endif
	return 0;
}