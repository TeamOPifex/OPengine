#include "./Core/include/OPdir.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef OPIFEX_WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#elif defined(OPIFEX_OSX)
	#include <mach-o/dyld.h>
#endif

#ifdef OPIFEX_OSX
	#include <libproc.h>
#endif

#ifdef OPIFEX_UNIX
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif

OPchar* OPdirCurrent() {
	OPchar cCurrentPath[FILENAME_MAX];
	OPchar* result;
	ui32 len;
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
    	return NULL;
    }

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	OPlogInfo("Working Directory: %s", cCurrentPath);

	len = (ui32)strlen(cCurrentPath) + 2;
	result = (OPchar*)OPalloc(sizeof(OPchar) * len);
	OPmemcpy(result, cCurrentPath, sizeof(OPchar)* (len - 2));
	result[len-2] = '\\';
	result[len-1] = '\0';
	return result;
}


OPchar* OPdirExecutable() {
#if defined(OPIFEX_WINDOWS) || defined(OPIFEX_OSX)
	char ownPth[1024]; //MAX_PATH - 260
	OPchar* result;
	ui32 len;
	char* pos = NULL;
#endif
#if defined(OPIFEX_OSX)
	char tmpPth[1024]; //MAX_PATH - 260
	i32 lastSlash;
#endif

	#ifdef OPIFEX_WINDOWS
		// Will contain exe path
		HMODULE hModule = GetModuleHandle(NULL);
		if (hModule != NULL)
		{
			// When passing NULL to GetModuleHandle, it returns handle of exe itself
			GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

			pos = strrchr(ownPth, '\\');
			if (pos != NULL) {
				*pos = '\0'; //this will put the null terminator here. you can also copy to another string if you want
			}
			OPlogInfo("Executable Directory: %s", ownPth);

			len = (ui32)strlen(ownPth) + 2;
			result = (OPchar*)OPalloc(sizeof(OPchar)* len);
			OPmemcpy(result, ownPth, sizeof(OPchar)* (len - 2));
			result[len - 2] = '\\';
			result[len - 1] = '\0';
			return result;
		}
	#elif defined(OPIFEX_OSX)
		lastSlash = -1;
		len = _NSGetExecutablePath(ownPth, &len);
		realpath(ownPth, tmpPth);

		len = strlen(tmpPth);
		for(ui32 i = 0; i < len; i++) {
			if(tmpPth[i] == '/') {
				lastSlash = i;
			}
		}
		if(lastSlash > -1) {
			tmpPth[lastSlash + 1] = '\0';
		}
		OPlog("The executable directory is \n%s\n%s", ownPth, tmpPth);

		len = strlen(tmpPth);
		result = (OPchar*)OPalloc(sizeof(OPchar)* len);
		OPmemcpy(result, tmpPth, sizeof(OPchar)* len);
		result[len] = '\0';

		return result;
	#else

	#endif
	return NULL;
}
