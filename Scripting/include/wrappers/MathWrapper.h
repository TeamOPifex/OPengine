#ifndef OPIFEX_SCRIPTING_WRAPPERS_MATH
#define OPIFEX_SCRIPTING_WRAPPERS_MATH

#ifdef OPIFEX_V8

#include "./Scripting/include/Scripting.h"

void MathInitializeMethods(V8isolate* isolate, V8ObjectT target);

#endif

#endif