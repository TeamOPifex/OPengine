#pragma once

struct OPsharedLibrarySymbol;
typedef struct OPsharedLibrarySymbol OPsharedLibrarySymbol;

#include "./Core/include/OPtypes.h"

struct OPsharedLibrarySymbol {
	void* Symbol;
	const OPchar* _symbolName;
};