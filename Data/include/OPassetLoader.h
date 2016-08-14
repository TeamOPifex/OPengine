#pragma once

struct OPassetLoader;
typedef struct OPassetLoader OPassetLoader;

#include "./Data/include/OPstream.h"

struct OPassetLoader {
	OPchar* Extension;
	OPchar* AssetTypePath;
	OPint AssetSize;
	OPint(*Load)(OPstream* stream, void** assetOut);
	OPint(*Unload)(void* assetIn);
	OPint(*Reload)(OPstream* stream, void** assetOut);
};