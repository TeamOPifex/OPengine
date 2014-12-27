#include "./Core/include/OPcore.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPtypes.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef OPIFEX_WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
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
	char ownPth[MAX_PATH];
	OPchar* result;
	ui16 len;

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth))); 
		
		char *pos = strrchr(ownPth, '\\');
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
	return NULL;
}