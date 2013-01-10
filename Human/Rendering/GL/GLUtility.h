#pragma once
#include "Core/include/Log.h"

class GLUtility {
public:
	// More than likely this should be replaced by an
	// initial check to see if there is an error
	// before passing a message through the process
	static bool CheckError(const char* message);
};