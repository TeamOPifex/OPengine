#pragma once

#ifndef OPEngine_Data_ContentMan
#define OPEngine_Data_ContentMan

#ifdef __cplusplus
extern "C"
{
#endif
#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"
#include "OPhashMap.h"

#define OP_CMAN_CAP 10000

//  _____ _                   _       
// / ____| |                 | |      
//| (___ | |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//                                                                      
typedef struct OPassetLoader{
	OPchar Extension[8];
	OPchar* AssetTypePath;
	OPint AssetSize;
	OPint (*Load)(const OPchar* path, void* assetOut);
	OPint (*Unload)(void* assetIn);
};

typedef struct OPasset{
	void* Asset;
	OPint (*Unload)(void* assetIn);
};

//  _____ _       _           _     
// / ____| |     | |         | |    
//| |  __| | ___ | |__   __ _| |___ 
//| | |_ | |/ _ \| '_ \ / _` | / __|
//| |__| | | (_) | |_) | (_| | \__ \
// \_____|_|\___/|_.__/ \__,_|_|___/
//                                  
extern HashMap OP_CMAN_HASHMAP;
extern OPassetLoader* OP_CMAN_ASSETLOADERS;
extern OPint OP_CMAN_ASSET_LOADER_COUNT;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                                                    
// Specifies how assets will be loaded for each file type
OPint OPcmanInit(OPassetLoader* loaders, OPint loaderCount);

// checks to see if an asset is loaded, triggers the load or unload.
OPint OPcmanIsLoaded(const OPchar* key);
OPint OPcmanLoad(const OPchar* key);
OPint OPcmanUnload(const OPchar* key);

// Returns a pointer to the asset requested by file name
void* OPcmanGet(const OPchar* key);

#ifdef __cplusplus
}
#endif

#endif
