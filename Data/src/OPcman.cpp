#include <include/OPhashMap.h>
#include "./Data/include/OPcman.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPvector.h"
#include "./Core/include/OPtimer.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPdir.h"
#include "./Core/include/OPcore.h"

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

OPuint OP_CMAN_RESOURCE_FILE_COUNT = 0;
OPresourceFile OP_CMAN_RESOURCE_FILES[OP_CMAN_MAX_RESOURCE_FILES];

OPlinkedList* OP_CMAN_PURGE;

OPlist* _OP_CMAN_ASSETLOADERS = NULL;

#if defined(_DEBUG)
#ifdef OPIFEX_WINDOWS

#endif
#include "./Data/include/OPstring.h"
i64 OP_CMAN_LAST_CHECKED = 100;

#endif

OPint OPcmanSetDir(OPchar* dir) {
	OPint result;
#if defined(OPIFEX_WINDOWS)
	result = SetCurrentDirectory(dir);
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	result = chdir(dir);
#else
	result = chdir(dir);
#endif
	return result;
}

void OPcmanUpdate(struct OPtimer* timer) {
#if defined(_DEBUG)
	i32 i, j;
	ui64 change;
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
				if (asset != NULL && asset->Reload != NULL) { // Only check the file, if there's a reload function
					change = OPfileLastChange(asset->AbsolutePath);
					if (change != asset->LastChange) {
						// OPlg("$, %s",asset->FullPath);
						OPstream* str = OPreadFileLarge(asset->FullPath, 1024);
						if (asset->Reload(str, &asset->Asset)) {
							asset->LastChange = change;
						}
						OPstreamDestroy(str);
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

	OPint result;
	OPint i;
	OPassetLoader* loader;

	result = 0;
	i = 0;


	if (_OP_CMAN_ASSETLOADERS == NULL) {
		OPlog("\n\n!!! NO ASSET LOADERS HAVE BEEN ADDED\n\n");
		_OP_CMAN_ASSETLOADERS = OPlistCreate(16, sizeof(OPassetLoader));
	}

	if (dir) {
		OP_CMAN_ASSET_FOLDER = OPstringCopy(dir);
	} else {
		OP_CMAN_ASSET_FOLDER = OPstringCreateMerged(OPEXECUTABLE_PATH, "assets/"); // OPstringCopy("assets/");
	}

	OP_CMAN_ASSET_LOADER_COUNT = OPlistSize(_OP_CMAN_ASSETLOADERS);
	OP_CMAN_ASSETLOADERS = (OPassetLoader*)OPalloc(sizeof(OPassetLoader)* OP_CMAN_ASSET_LOADER_COUNT);

	for (i = 0; i < OP_CMAN_ASSET_LOADER_COUNT; i++) {
		loader = (OPassetLoader*)OPlistGet(_OP_CMAN_ASSETLOADERS, i);
		OPmemcpy(&OP_CMAN_ASSETLOADERS[i], loader, sizeof(OPassetLoader));
	}

	OPlistDestroy(_OP_CMAN_ASSETLOADERS);
	OPfree(_OP_CMAN_ASSETLOADERS);
	_OP_CMAN_ASSETLOADERS = NULL;

	// Switch to the assets directory
#if defined(OPIFEX_WINDOWS)
	result = SetCurrentDirectory(OP_CMAN_ASSET_FOLDER);

	if (result) {
		OPlogInfo("Directory Changed: %s", OP_CMAN_ASSET_FOLDER);
	}
	else {
		OPlogErr("!Directory Change Failed: %s", OP_CMAN_ASSET_FOLDER);
	}
#else
	result = chdir(OP_CMAN_ASSET_FOLDER);

	if (result == 0) {
		OPlog("Directory Changed to: %s", OP_CMAN_ASSET_FOLDER);
	}
	else {
		OPlog("!Directory Change Failed: %s", OP_CMAN_ASSET_FOLDER);
	}
#endif


#ifdef _DEBUG
	//OPfree(OP_CMAN_ASSET_FOLDER);
	//OP_CMAN_ASSET_FOLDER = OPdirCurrent();
#endif


	// create and copy the hashmap
	OP_CMAN_HASHMAP.Init(OP_CMAN_CAP);

	// create and copy the purge list
	OP_CMAN_PURGE = OPllCreate();

	return 1;
}

// checks to see if an asset is loaded, triggers the load or unload.
OPint OPcmanIsLoaded(const OPchar* key){
	return OP_CMAN_HASHMAP.Exists(key);
}

OPint OPcmanLoad(const OPchar* key){

	const OPchar* ext = NULL;
	OPint success = 0;
	if(OP_CMAN_HASHMAP.Exists(key)){
		OPasset* existing = NULL;
		OP_CMAN_HASHMAP.Get(key, (void**)&existing);

		// the key may have been added, but see if there
		// is any valid pointer in the bucket before declaring it exists
		if(existing){
			existing->Dirty = 0;
			return OP_CMAN_KEY_EXISTS;
		}
	}



	ext = strrchr(key, '.');
	if(ext){
		OPint i = 0, extLen = (OPuint)strlen(ext);
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
				len = (OPuint)strlen(loader.AssetTypePath) + (OPuint)strlen(key);
				fullPath = (OPchar*)OPalloc(sizeof(OPchar) * len + 1);
				OPbzero(fullPath, len);
				if(!fullPath) return OP_CMAN_PATH_ALLOC_FAILED;
				fullPath = strcat(fullPath, loader.AssetTypePath);
				fullPath = strcat(fullPath, key);

				// load the asset
				OPstream* str = OPcmanGetResource(fullPath);
				if (str == NULL) {
					str = OPreadFileLarge(fullPath, 1024);
				}
				success = loader.Load(str, &asset);
				OPstreamDestroy(str);
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
#if defined(_DEBUG)
				assetBucket->Reload = loader.Reload;
				assetBucket->FullPath = fullPath;
				assetBucket->AbsolutePath = OPstringCreateMerged(OP_CMAN_ASSET_FOLDER, fullPath);
				assetBucket->LastChange = OPfileLastChange(assetBucket->AbsolutePath);
#endif

				// finally insert into the hashmap
				if(OP_CMAN_HASHMAP.Put(key, assetBucket))
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

	if(!OP_CMAN_HASHMAP.Exists(key))
		return OP_CMAN_KEY_NOT_FOUND;

	if(!OP_CMAN_HASHMAP.Get(key, &value))
		return OP_CMAN_RETRIEVE_FAILED;
	asset = (OPasset*)value;

	if(!value) return OP_CMAN_ASSET_LOAD_FAILED;

	if(!asset->Unload(asset->Asset)) return 0;
	OP_CMAN_HASHMAP.Put(key, NULL);
	OPfree(asset); asset = NULL;

	return 1;
}

// Returns a pointer to the asset requested by file name
void* OPcmanGet(const OPchar* key){
	OPasset* bucket = NULL;
	if(!OP_CMAN_HASHMAP.Exists(key)) {
		OPlog("Asset has not been loaded. '%s'", key);
		return NULL;
	}
	OP_CMAN_HASHMAP.Get(key, (void**)&bucket);
	return bucket->Asset;
}

OPint OPcmanDelete(const OPchar* key){
	OPasset* bucket = NULL;
	OP_CMAN_HASHMAP.Get(key, (void**)&bucket);
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
#if defined(_DEBUG)
			OPfree(bucket->AbsolutePath);
			OPfree(bucket->FullPath);
#endif
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

void OPcmanDestroy() {

	Bucket* bucket;
	OPuint i, j, n, m;
	KeyValuePair *pair;

	n = OP_CMAN_HASHMAP.count;
	bucket = OP_CMAN_HASHMAP.buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		j = 0;
		while(j < m) {
			OPcmanDelete(pair->key);
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	OPcmanPurge();

	if (_OP_CMAN_ASSETLOADERS) {
		OPlistDestroy(_OP_CMAN_ASSETLOADERS);
		OPfree(_OP_CMAN_ASSETLOADERS);
	}

	OPfree(OP_CMAN_ASSET_FOLDER);
	OPfree(OP_CMAN_ASSETLOADERS);
	OP_CMAN_HASHMAP.Destroy();
	if (OP_CMAN_PURGE) {
		OPllDestroy(OP_CMAN_PURGE);
		OPfree(OP_CMAN_PURGE);
	}
}

void OPcmanLoadResourcePack(const OPchar* filename) {
	// Grab the next OPresourceFile slot available
	OPresourceFile* resource = &OP_CMAN_RESOURCE_FILES[OP_CMAN_RESOURCE_FILE_COUNT++];

	// Opens the File handle
	// The file handle will stay open until the resourceFile is unloaded
	resource->resourceFile = OPfileOpen(filename);

	// Get the resourceFile version
	ui8 version = OPfileReadui8(&resource->resourceFile);
	OPlogDebug("Version %d", version);

	// The number of resources in this pack
	resource->resourceCount = OPfileReadui16(&resource->resourceFile);

	// The total length of all names
	// This is used to make a contiguous block of OPchar data so that
	// the lookup of resources is faster
	ui32 lengthOfNames = OPfileReadui32(&resource->resourceFile);

	// Allocate the necessary data
	// TODO: (garrett) allocate this into a single struct so that there's only 1 allocation
	resource->resourceNames = (OPchar**)OPalloc(sizeof(OPchar*)* resource->resourceCount);
	resource->resourceOffset = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);
	resource->resourceSize = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);

	OPlogDebug("Resource Count %d", resource->resourceCount);

	for (ui16 i = 0; i < resource->resourceCount; i++) {
		// TODO: (garrett) Read in the string into a contiguous block of OPchar data
		resource->resourceNames[i] = OPfileReadString(&resource->resourceFile);
		resource->resourceOffset[i] = OPfileReadui32(&resource->resourceFile);
		resource->resourceSize[i] = OPfileReadui32(&resource->resourceFile);

		OPlogDebug("Resource %s", resource->resourceNames[i]);
	}
}

OPstream* OPcmanGetResource(const OPchar* resourceName) {
	// Loop through all currently loaded resource packs
	for (ui16 i = 0; i < OP_CMAN_RESOURCE_FILE_COUNT; i++) {
		OPresourceFile resource = OP_CMAN_RESOURCE_FILES[i];
		// Loop through each resource in the pack
		for (ui16 j = 0; j < resource.resourceCount; j++) {
			// Try to find the resource requested
			if (!OPstringEquals(resourceName, resource.resourceNames[j])) {
				continue;
			}

			// The resource was found in this pack
			// Set the seek position into the file
			// Then load it into an OPstream
			ui32 offset = resource.resourceOffset[j];
			ui32 size = resource.resourceSize[j];

			// TODO: (garrett) This should be done in a way that will support threading
			OPfileSeek(&resource.resourceFile, offset);
			OPstream* stream = OPfileRead(&resource.resourceFile, size);
			stream->Source = resource.resourceNames[j];

			return stream;
		}
	}
	return NULL;
}
