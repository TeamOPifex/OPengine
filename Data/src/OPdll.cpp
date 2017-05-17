#include "./Data/include/OPdll.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPdir.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPcore.h"

#ifdef OPIFEX_UNIX
	#include <dlfcn.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif

void OPdll::Init(const OPchar* path) {
#if defined(OPIFEX_WINDOWS)
#else
	OPchar* pathMerge = OPstringCreateMerged(OPEXECUTABLE_PATH, path);

	library = dlopen(pathMerge, RTLD_LAZY);

	if (library == NULL) {
		OPlogErr("FAILED TO LOAD LIBRARY");
	}

	path = pathMerge;
    #if _DEBUG
	lastModified = OPfile::LastChange(pathMerge);
    #endif
#endif
}

OPint OPdll::Update() {

    #if _DEBUG
#if defined(OPIFEX_WINDOWS)
#else
	ui64 lastChange = OPfile::LastChange(path);
	if (lastModified == lastChange) return 0;
	lastModified = lastChange;

	Destroy();
	library = NULL;
	while (library == NULL) {
		library = dlopen(path, RTLD_LAZY);

		if (library == NULL) {
			OPlogErr("FAILED TO LOAD LIBRARY");
		}
	}
#endif
#endif
	return 1;
}

void* OPdll::Symbol(const OPchar* symbol) {
	void* result = NULL;
#if defined(OPIFEX_WINDOWS)
#else
	result = dlsym(library, symbol);
	Dl_info info;
	if (dladdr(result, &info)) {
		OPlog("Info on dependencies():\n");
		OPlog("    Pathname: %s\n", info.dli_fname);
		OPlog("    Base address: %p\n", info.dli_fbase);
		OPlog("    Nearest symbol: %s\n", info.dli_sname);
		OPlog("    Symbol address: %p\n", info.dli_saddr);
	}
	else {
		OPlog("No valid data");
	}
#endif
	return result;
}

void OPdll::Destroy() {
#if defined(OPIFEX_WINDOWS)
#else
	dlclose(library);
#endif
}
