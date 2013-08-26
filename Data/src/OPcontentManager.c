#include "./Data/include/OPcontentManager.h"

HashMap OP_CMAN_HASHMAP;
OPassetLoader* OP_CMAN_ASSETLOADERS;
OPint OP_CMAN_ASSET_LOADER_COUNT;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                                                    
// Specifies how assets will be loaded for each file type
OPint OPcmanInit(OPassetLoader* loaders, OPint loaderCount){
	OP_CMAN_ASSETLOADERS = loaders;
	OP_CMAN_ASSET_LOADER_COUNT = loaderCount;
	
	// create and copy the hashmap
	OPmemcpy(OPhashMapCreate(OP_CMAN_CAP), &OP_CMAN_HASHMAP, sizeof(HashMap));
}

// checks to see if an asset is loaded, triggers the load or unload.
OPint OPcmanIsLoaded(const OPchar* key){
	return OPhashMapExists(&OP_CMAN_HASHMAP, key);
}

OPint OPcmanLoad(const OPchar* key){
	if(OPhashMapExists(&OP_CMAN_HASHMAP, key))
		return -1;

	const OPchar* ext = strrchr(key, '.');
	if(ext){
		OPint extLen = strlen(ext); extLen = extLen <= 8 ? extLen : 8;
		for(OPint i = OP_CMAN_ASSET_LOADER_COUNT; i--;){
			if(OPmemcmp(OP_CMAN_ASSETLOADERS[i].Extension, ext, extLen) == 0){
				// the correct loader for this file type has been found

			}
		}
		return -2;
	}

	return -1;
}
OPint OPcmanUnload(const OPchar* key){

}

// Returns a pointer to the asset requested by file name
void* OPcmanGet(const OPchar* key){

}