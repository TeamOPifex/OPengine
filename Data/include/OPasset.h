#pragma once

struct OPasset;
typedef struct OPasset OPasset;

#include "./Data/include/OPstream.h"
#include "./Data/include/OPassetLoader.h"

struct OPasset {
	void* Asset;
	OPint(*Unload)(void* assetIn);
	OPint Dirty;

#if defined(_DEBUG)
	OPint(*Reload)(OPstream* stream, void** assetOut) = NULL;
	OPchar* FullPath = NULL;
	i64 LastChange = 0;
#endif

	OPasset() { }
	OPasset(void* asset, OPassetLoader* assetLoader, OPchar* pathToAsset) { Init(asset, assetLoader, pathToAsset); }
	
	void Init(void* asset, OPassetLoader* assetLoader, OPchar* pathToAsset);
	void Destroy();

	inline OPasset* Create(void* asset, OPassetLoader* assetLoader, OPchar* pathToAsset) {
		return OPNEW(OPasset(asset, assetLoader, pathToAsset));
	}
};