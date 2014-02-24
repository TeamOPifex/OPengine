#include "./Data/include/ContentManager.h"
#include "./Core/include/Log.h"

//  _____ _       _           _     
// / ____| |     | |         | |    
//| |  __| | ___ | |__   __ _| |___ 
//| | |_ | |/ _ \| '_ \ / _` | / __|
//| |__| | | (_) | |_) | (_| | \__ \
// \_____|_|\___/|_.__/ \__,_|_|___/
//                                  
HashMap OP_CMAN_HASHMAP;
OPassetLoader* OP_CMAN_ASSETLOADERS;
OPint OP_CMAN_ASSET_LOADER_COUNT;

OPlinkedList* OP_CMAN_PURGE;

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

	// Switch to the assets directory
#if defined(OPIFEX_WINDOWS)
	_chdir("assets\\");
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	if(chdir("./assets")) OPlog("Directory changed!\n");
	else OPlog("Directory change failed!!!\n");
#else
	if(chdir("./assets") == 0) OPlog("Directory changed!\n");
	else OPlog("Directory change failed!!!\n");
#endif

	
	// create and copy the hashmap
	OPmemcpy(&OP_CMAN_HASHMAP, OPhashMapCreate(OP_CMAN_CAP), sizeof(HashMap));

	// create and copy the purge list
	OP_CMAN_PURGE = OPllCreate();

	return 1;
}

// checks to see if an asset is loaded, triggers the load or unload.
OPint OPcmanIsLoaded(const OPchar* key){
	return OPhashMapExists(&OP_CMAN_HASHMAP, key);
}

OPint OPcmanLoad(const OPchar* key){
	const OPchar* ext = NULL;
	OPint success = 0;
	if(OPhashMapExists(&OP_CMAN_HASHMAP, key)){
		OPasset* existing = NULL;
		OPhashMapGet(&OP_CMAN_HASHMAP, key, (void**)&existing);

		// the key may have been added, but see if there
		// is any valid pointer in the bucket before declaring it exists
		if(existing){
			existing->Dirty = 0;
			return OP_CMAN_KEY_EXISTS;
		}
	}

	ext = strrchr(key, '.');
	if(ext){
		OPint i = 0, extLen = strlen(ext);
		extLen = extLen <= 8 ? extLen : 8;

		for(i = OP_CMAN_ASSET_LOADER_COUNT; i--;){
			if(OPmemcmp(OP_CMAN_ASSETLOADERS[i].Extension, ext, extLen) == 0){
				OPasset* assetBucket = NULL;
				OPchar* fullPath = NULL;
				void* asset = NULL;
				OPint len = 0;
				// the correct loader for this file type has been found
				OPassetLoader loader = OP_CMAN_ASSETLOADERS[i];

				// build the path string
				len = strlen(loader.AssetTypePath) + strlen(key);
				fullPath = (OPchar*)OPalloc(sizeof(OPchar) * len + 1);
				OPbzero(fullPath, len);
				if(!fullPath) return OP_CMAN_PATH_ALLOC_FAILED;
				fullPath = strcat(fullPath, loader.AssetTypePath);
				fullPath = strcat(fullPath, key);

				// load the asset
				asset = NULL;
				success = loader.Load(fullPath, &asset);
				if(success <= 0) return OP_CMAN_ASSET_LOAD_FAILED;

				// clean up the string
				OPfree(fullPath);

				// create the asset to insert into the hashmap
				if(!(assetBucket = (OPasset*)OPalloc(sizeof(OPasset))))
					return OP_CMAN_BUCKET_ALLOC_FAILED;
				assetBucket->Asset = asset;
				assetBucket->Unload = loader.Unload;
				assetBucket->Dirty = 0;
				
				// finally insert into the hashmap
				if(OPhashMapPut(&OP_CMAN_HASHMAP, key, assetBucket))
					return 1;

				return OP_CMAN_INSERTION_FAILED;
			}
		}
		return OP_CMAN_NO_MATCHING_UPLOADER;
	}

	return OP_CMAN_EXT_NOT_MATCHED;
}
OPint OPcmanUnload(const OPchar* key){
	void* value = NULL;
	OPasset* asset = NULL;

	if(!OPhashMapExists(&OP_CMAN_HASHMAP, key))
		return OP_CMAN_KEY_NOT_FOUND;

	if(!OPhashMapGet(&OP_CMAN_HASHMAP, key, &value))
		return OP_CMAN_RETRIEVE_FAILED;
	asset = (OPasset*)value;

	if(!value) return OP_CMAN_ASSET_LOAD_FAILED;

	if(!asset->Unload(asset->Asset)) return 0;
	OPhashMapPut(&OP_CMAN_HASHMAP, key, NULL);
	OPfree(asset); asset = NULL;

	return 1;
}

// Returns a pointer to the asset requested by file name
void* OPcmanGet(const OPchar* key){
	OPasset* bucket = NULL;
	OPhashMapGet(&OP_CMAN_HASHMAP, key, (void**)&bucket);
	return bucket->Asset;
}

OPint OPcmanDelete(const OPchar* key){
	OPasset* bucket = NULL;
	OPhashMapGet(&OP_CMAN_HASHMAP, key, (void**)&bucket);
	OPllInsertLast(OP_CMAN_PURGE, bucket);
	return bucket->Dirty = 1;
}

OPint OPcmanPurge(){
	OPllNode* n = OP_CMAN_PURGE->First;
	while (n){
		OPasset* bucket = (OPasset*)n->Data;
		OPllNode* next = n->Next;
		if(bucket->Dirty){
			if(!bucket->Unload(bucket->Asset))
				return 0;
			OPfree(bucket);
			bucket = NULL;
		}
		OPllRemove(OP_CMAN_PURGE, n);
		n = next;
	}
	return 1;
}
