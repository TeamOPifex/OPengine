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

	len = strlen(cCurrentPath);
	result = (OPchar*)OPalloc(sizeof(OPchar) * len);
	OPmemcpy(result, cCurrentPath, sizeof(OPchar) * len);
	return result;
}