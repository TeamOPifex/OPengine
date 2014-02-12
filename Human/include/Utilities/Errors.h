#ifndef OPENGINE_HUMAN_UTILITIES_ERRORS
#define OPENGINE_HUMAN_UTILITIES_ERRORS

#include "./Core/include/Types.h"
#include "Core/include/Log.h"

// More than likely this should be replaced by an
// initial check to see if there is an error
// before passing a message through the process
OPint CheckError(const char* message);

#endif