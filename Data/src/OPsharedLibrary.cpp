#include "./Data/include/OPsharedLibrary.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"

// TODO: abstract out to Windows and Linux
#include <dlfcn.h>
#include <sys/stat.h> 
#include <unistd.h>

OPsharedLibrary* OPsharedLibraryLoad(const OPchar* path) {
	void* library = dlopen(path, RTLD_NOW);
	if(library == NULL) return NULL;

	OPsharedLibrary* sharedLibrary = (OPsharedLibrary*)OPalloc(sizeof(OPsharedLibrary));
	sharedLibrary->_library = library;
	sharedLibrary->_libraryPath = path;
	sharedLibrary->_symbols = OPlistCreate(4, sizeof(OPsharedLibrarySymbol));

	return sharedLibrary;
}

OPint OPsharedLibraryReload(OPsharedLibrary* sharedLibrary) {

	ui64 lastChange = OPfileLastChange(sharedLibrary->_libraryPath);
	if(sharedLibrary->_lastModifiedTime == lastChange) return 0;

	if(OPsharedLibraryClose(sharedLibrary) != 0) {
		return -1;
	}

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
		}
		sharedLibrarySymbol->Symbol = symbol;
	}

	return result;
}

OPsharedLibrarySymbol* OPsharedLibraryLoadSymbol(OPsharedLibrary* sharedLibrary, const OPchar* symbolName) {
	void* symbol = dlsym(sharedLibrary->_library, symbolName);
	if(symbol == NULL) return NULL;

	OPsharedLibrarySymbol sharedLibrarySymbol = {
		symbol,
		symbolName
	};
	OPlistPush(sharedLibrary->_symbols, (ui8*)&sharedLibrarySymbol);
	return (OPsharedLibrarySymbol*)OPlistPeek(sharedLibrary->_symbols);
}

OPint OPsharedLibraryClose(OPsharedLibrary* sharedLibrary) {
	OPint result = dlclose(sharedLibrary->_library);
	if(result != 0) return -1;
	return 0;
}