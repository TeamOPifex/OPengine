#pragma once

struct OPresourceFile;
typedef struct OPresourceFile OPresourceFile;

#include "./Data/include/OPfile.h"

struct OPresourceFile {
	OPfile resourceFile;
	ui16 headerSize;
	ui16 resourceCount;
	OPchar** resourceNames;
	ui32* resourceOffset;
	ui32* resourceSize;
};