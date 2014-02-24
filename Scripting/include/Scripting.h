#ifndef OPIFEX_SCRIPTING
#define OPIFEX_SCRIPTING

#include "./Human/include/Utilities/LoaderOPS.h"

void OPscriptInit();
void OPscriptCompile(OPscript* script);
void OPscriptLog(const char* data); 
void OPscriptRun(const char* func);
#endif