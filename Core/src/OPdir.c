#include "./Core/include/OPcore.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPtypes.h"

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
	ui16 len;
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
    	return NULL;
    }

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	OPlog("The current working directory is %s", cCurrentPath);

	len = strlen(cCurrentPath) + 1;
	result = (OPchar*)OPalloc(sizeof(OPchar) * len);
	OPmemcpy(result, cCurrentPath, sizeof(OPchar)* (len - 1));
	result[len-1] = '\\';
	result[len] = NULL;
	return result;
}


OPchar* OPdirExecutable() {
	char ownPth[1024]; //MAX_PATH - 260
	char tmpPth[1024]; //MAX_PATH - 260
	OPchar* result;
	ui32 len;
	char* pos = NULL;

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
			OPlog("The executable directory is %s", ownPth);

			len = strlen(ownPth) + 1;
			result = (OPchar*)OPalloc(sizeof(OPchar)* len);
			OPmemcpy(result, ownPth, sizeof(OPchar)* (len - 1));
			result[len - 1] = '\\';
			result[len] = NULL;
			return result;
		}
	#elif defined(OPIFEX_OSX)
		len = _NSGetExecutablePath(ownPth, &len);
		realpath(ownPth, tmpPth);

		len = strlen(tmpPth);
		if(len < 1024) {
			tmpPth[len] = '/';
			tmpPth[len + 1] = NULL; 
		}
		OPlog("The executable directory is \n%s\n%s", ownPth, tmpPth);

		len = strlen(tmpPth);
		result = (OPchar*)OPalloc(sizeof(OPchar)* len);
		OPmemcpy(result, tmpPth, sizeof(OPchar)* len);
		result[len] = NULL;

		return result;
	#else

	#endif
	return NULL;
}