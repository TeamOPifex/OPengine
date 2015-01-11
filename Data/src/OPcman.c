#include "./Data/include/OPcman.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPvector.h"
#include "./Core/include/OPtimer.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPstring.h"

//  _____ _       _           _     
// / ____| |     | |         | |    
//| |  __| | ___ | |__   __ _| |___ 
//| | |_ | |/ _ \| '_ \ / _` | / __|
//| |__| | | (_) | |_) | (_| | \__ \
// \_____|_|\___/|_.__/ \__,_|_|___/
//                                  
OPhashMap OP_CMAN_HASHMAP;
OPassetLoader* OP_CMAN_ASSETLOADERS;
OPint OP_CMAN_ASSET_LOADER_COUNT;
OPchar* OP_CMAN_ASSET_FOLDER;

OPlinkedList* OP_CMAN_PURGE;

OPlist* _OP_CMAN_ASSETLOADERS = NULL;

#if defined(_DEBUG) && defined(OPIFEX_WINDOWS)
#define BUFSIZE MAX_PATH
#include <windows.h>
#include "./Data/include/OPstring.h"
i64 OP_CMAN_LAST_CHECKED = 100;

#endif

void OPcmanUpdate(OPtimer* timer) {
#if defined(_DEBUG) && defined(OPIFEX_WINDOWS)
	i32 i, j;
	long change;
	Bucket bucket;
	OPasset* asset;
	OP_CMAN_LAST_CHECKED -= timer->Elapsed;
	if (OP_CMAN_LAST_CHECKED < 0) {
		// Only check for file changes once per second
		OP_CMAN_LAST_CHECKED = 100;
		for (i = 0; i < OP_CMAN_HASHMAP.count; i++) {
			bucket = OP_CMAN_HASHMAP.buckets[i];
			for (j = 0; j < bucket.count; j++) {
				asset = (OPasset*)bucket.pairs[j].value;
				if (asset != NULL && asset->Reload) { // Only check the file, if there's a reload function
					change = OPfileLastChange(asset->AbsolutePath);
					if (change != asset->LastChange) {
						if (asset->Reload(asset->FullPath, &asset->Asset)) {
							asset->LastChange = change;
						}
					}
				}
			}
		}
	}
#endif
}

void OPcmanAddLoader(OPassetLoader* loader) {
	if (_OP_CMAN_ASSETLOADERS == NULL) {
		_OP_CMAN_ASSETLOADERS = OPlistCreate(16, sizeof(OPassetLoader));
	}
	OPlistPush(_OP_CMAN_ASSETLOADERS, (ui8*)loader);
}

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                                                    
// Specifies how assets will be loaded for each file type
OPint OPcmanInit(const OPchar* dir){

#if defined(_DEBUG) && defined(OPIFEX_WINDOWS)
	TCHAR Buffer[BUFSIZE];
	DWORD dwRet;
#endif

	OPint result;
	OPint i;
	OPassetLoader* loader;

	result = 0; 
	i = 0;


	if (_OP_CMAN_ASSETLOADERS == NULL) {
		OPlog("\n\n!!! NO ASSET LOADERS HAVE BEEN ADDED\n\n");
		_OP_CMAN_ASSETLOADERS = OPlistCreate(16, sizeof(OPassetLoader));
	}

	OP_CMAN_ASSET_FOLDER = "assets\\";
	if (dir) {
		OP_CMAN_ASSET_FOLDER = OPstringCopy(dir);
	}

	OP_CMAN_ASSET_LOADER_COUNT = OPlistSize(_OP_CMAN_ASSETLOADERS);
	OP_CMAN_ASSETLOADERS = (OPassetLoader*)OPalloc(sizeof(OPassetLoader)* OP_CMAN_ASSET_LOADER_COUNT);

	for (i = 0; i < OP_CMAN_ASSET_LOADER_COUNT; i++) {
		loader = (OPassetLoader*)OPlistGet(_OP_CMAN_ASSETLOADERS, i);
		OPmemcpy(&OP_CMAN_ASSETLOADERS[i], loader, sizeof(OPassetLoader));
	}

	OPfree(_OP_CMAN_ASSETLOADERS);
	_OP_CMAN_ASSETLOADERS = NULL;

	// Switch to the assets directory
#if defined(OPIFEX_WINDOWS)
	result = _chdir(OP_CMAN_ASSET_FOLDER);
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	result = chdir(OP_CMAN_ASSET_FOLDER);
#else
	result = chdir(OP_CMAN_ASSET_FOLDER);
#endif

	if (result == 0) {
		OPlog("Directory Changed to: %s", OP_CMAN_ASSET_FOLDER);
	}
	else {
		OPlog("!Directory Change Failed: %s", OP_CMAN_ASSET_FOLDER);
	}


#ifdef _DEBUG
#ifdef OPIFEX_WINDOWS
	dwRet = GetCurrentDirectory(BUFSIZE, Buffer);
	OP_CMAN_ASSET_FOLDER = OPstringCreateMerged(Buffer, "\\");
#endif
#endif

	
	// create and copy the hashmap
	OPmemcpy(&OP_CMAN_HASHMAP, OPhashMapCreate(OP_CMAN_CAP), sizeof(OPhashMap));

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
		
		for (i = OP_CMAN_ASSET_LOADER_COUNT; i--;){
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
				if(success <= 0) {
					OPlog("Failed to load %s", fullPath);
					OPfree(fullPath);
					return OP_CMAN_ASSET_LOAD_FAILED;
				}



				// create the asset to insert into the hashmap
				if (!(assetBucket = (OPasset*)OPalloc(sizeof(OPasset)))) {

					OPlog("Failed to allocate bucket for %s", fullPath);

					// clean up the string
					OPfree(fullPath);

					return OP_CMAN_BUCKET_ALLOC_FAILED;
				}
				assetBucket->Asset = asset;
				assetBucket->Unload = loader.Unload;
				assetBucket->Dirty = 0;
#if defined(_DEBUG) && defined(OPIFEX_WINDOWS)
				assetBucket->Reload = loader.Reload;
				assetBucket->FullPath = fullPath;
				assetBucket->AbsolutePath = OPstringCreateMerged(OP_CMAN_ASSET_FOLDER, fullPath);
				assetBucket->LastChange = OPfileLastChange(assetBucket->AbsolutePath);
#endif
				
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
	if(!OPhashMapExists(&OP_CMAN_HASHMAP, key)) {
		OPlog("Asset has not been loaded. '%s'", key);
		return NULL;
	}
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

void* OPcmanLoadGet(const OPchar* key) {
	OPint result;
	result = OPcmanLoad(key);
	if (!result) {
		return NULL;
	}
	return OPcmanGet(key);
}
