#include "./Data/include/OPdll.h"

void OPdll::Init(const OPchar* path) {
#if defined(OPIFEX_WINDOWS)
#else
	OPchar* pathMerge = OPstringCreateMerged(OPgetExecutableDir(), path);

	library = dlopen(pathMerge, RTLD_LAZY);

	if (library == NULL) {
		OPlogErr("FAILED TO LOAD LIBRARY");
	}

	path = pathMerge;
	lastModified = OPfileLastChange(pathMerge);
#endif
}

OPint OPdll::Update() {
#if defined(OPIFEX_WINDOWS)
#else
	ui64 lastChange = OPfileLastChange(path);
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
