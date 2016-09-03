#include "./Data/include/OPcman.h"
#include "./Data/include/OPresourceFile.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPdebug.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPdir.h"
#include "./Core/include/OPcore.h"
#include "./Core/include/Assert.h"

OPcman OPCMAN;

bool OPcman::Init(const OPchar* dir) {
	TIMED_BLOCK;

	assetLoaders.Init(sizeof(OPassetLoader));
	resourceFiles.Init(sizeof(OPresourceFile));
	purgeList.Init();
	hashmap.Init(OP_CMAN_CAP);
	
	if (dir == NULL) {
		assetDirectoriesCount = 0;
		rootFolder = OPstringCreateMerged(OPEXECUTABLE_PATH, "assets/");
	}
	else {
		assetDirectoriesCount = OPstringSplit(dir, '|', &assetDirectories);
		for (ui32 i = 0; i < assetDirectoriesCount; i++) {
			OPlogInfo("String Split: %s", assetDirectories[i]);
		}
		rootFolder = OPstringCopy(assetDirectories[0]);
	}

	i32 result;

	// Switch to the assets directory
#if defined(OPIFEX_WINDOWS)
	result = SetCurrentDirectory(rootFolder);

	if (result) {
		OPlogInfo("Directory Changed: %s", rootFolder);
	}
	else {
		OPlogErr("!Directory Change Failed: %s", rootFolder);
	}
#else
	result = chdir(rootFolder);

	if (result == 0) {
		OPlog("Directory Changed to: %s", rootFolder);
	}
	else {
		OPlog("!Directory Change Failed: %s", rootFolder);
	}
#endif

	return true;
}

bool OPcman::Add(const OPchar* assetKey, OPasset* asset) {
	hashmap.Put(assetKey, asset);
	return true;
}

void OPcman::AddLoader(OPassetLoader* loader) {
	assetLoaders.Push((ui8*)loader);
}

void OPcman::Update(OPtimer* timer) {
	#if defined(_DEBUG)
		i32 i, j;
		ui64 change;
		OPhashMapBucket* bucket;
		OPasset* asset;
		OPuint n, m;
		OPhashMapPair *pair;

		lastChecked -= timer->Elapsed;
		if (lastChecked > 0) return;

		// Only check for file changes once per second
		lastChecked = 1000;


		n = hashmap.count;
		bucket = hashmap.buckets;
		i = 0;
		while (i < n) {
			m = bucket->count;
			pair = bucket->pairs;
			j = 0;
			while (j < m) {
				asset = (OPasset*)pair->value;
				if (asset != NULL && asset->Reload != NULL) {
					change = OPfile::LastChange(asset->FullPath);
					if (change != asset->LastChange) {
						OPlogInfo("Reloading Asset: %s", asset->FullPath);

						OPstream* str = OPfile::ReadFromFile(asset->FullPath, 1024);
						if (asset->Reload(str, &asset->Asset)) {
							asset->LastChange = change;
						}
						str->Destroy();
						OPfree(str);
					}
				}

				pair++;
				j++;
			}
			bucket++;
			i++;
		}
	#endif
}

bool OPcman::Purge() {
		OPlinkedListNode* n = purgeList.First;
		while (n){
			OPasset* asset = (OPasset*)n->Data;
			if(asset->Dirty){
				asset->Destroy();
				OPfree(asset);
				n->Data = NULL;
			}
			OPlinkedListNode* next = n->Next;
			purgeList.Remove(n);
			n = next;
		}
		return 1;
	return false;
}

void* OPcman::LoadFromFile(const OPchar* path) {
	const OPchar* ext = NULL;
	OPint success = 0;

	ext = strrchr(path, '.');
	ASSERT(ext != NULL, "Finding extension failed");

	for (OPint i = 0; i < assetLoaders.Size(); i++) {
		OPassetLoader* loader = (OPassetLoader*)assetLoaders.Get(i);
		if (!OPstringEquals(loader->Extension, ext)) {
			continue;
		}

		OPstream* str = OPfile::ReadFromFile(path, 1024);

		void* assetPtr = NULL;
		OPint loadResult = loader->Load(str, &assetPtr);
		str->Destroy();
		OPfree(str);
		if (loadResult <= 0) {
			OPlogErr("Failed to load asset: %s", path);
			return NULL;
		}

		return assetPtr;
	}

	return NULL;
}

bool OPcman::Load(const OPchar* assetKey) {
	const OPchar* ext = NULL;
	OPint success = 0;

	if(hashmap.Exists(assetKey)){
		OPasset* existing = NULL;
		hashmap.Get(assetKey, (void**)&existing);

		ASSERT(existing != NULL, "Hashmap falsely claimed asset existed");
		return true;
	}

	ext = strrchr(assetKey, '.');
	ASSERT(ext != NULL, "Finding extension failed");

	for (OPint i = 0; i < assetLoaders.Size(); i++) {
		OPassetLoader* loader = (OPassetLoader*)assetLoaders.Get(i);
		if (!OPstringEquals(loader->Extension, ext)) {
			continue;
		}

		
		// Found the correct loader for this asset extension type
		OPchar* fullPathToAsset = OPstringCreateMerged(loader->AssetTypePath, assetKey);
		if (!OPfile::Exists(fullPathToAsset)) {
			for (ui32 j = 1; j < assetDirectoriesCount; j++) {
				OPchar* absolutePath = OPstringCreateMerged(assetDirectories[j], fullPathToAsset);
				if (OPfile::Exists(absolutePath)) {
					OPfree(fullPathToAsset);
					fullPathToAsset = absolutePath;
					break;
				} else {
					OPfree(absolutePath);
				}
			}
		}
		OPstream* str = GetResource(fullPathToAsset);
		if (str == NULL) {
			str = OPfile::ReadFromFile(fullPathToAsset, 1024);
		}

		void* assetPtr;
		OPint loadResult = loader->Load(str, &assetPtr);
		str->Destroy();
		OPfree(str);
		if (loadResult <= 0) {
			OPlogErr("Failed to load asset: %s", fullPathToAsset);
			return false;
		}

		OPasset* asset = OPNEW(OPasset(assetPtr, loader, fullPathToAsset));
		OPfree(fullPathToAsset);

		if (hashmap.Put(assetKey, asset) > 0) {
			return true;
		}
	}

	return false;
}

bool OPcman::Unload(const OPchar* assetKey) {
		void* value = NULL;
		OPasset* asset = NULL;

		if(!hashmap.Exists(assetKey))
			return false;

		if(!hashmap.Get(assetKey, &value))
			return false;

		ASSERT(value != NULL, "Hashmap failed to get asset value");

		asset = (OPasset*)value;

		asset->Destroy();
		OPfree(asset);
		asset = NULL;

		hashmap.Remove(assetKey);

		return 1;
	return false;
}

void* OPcman::Get(const OPchar* assetKey) {
	OPasset* bucket = NULL;
	ASSERT(hashmap.Exists(assetKey), "Asset has not been loaded");
	hashmap.Get(assetKey, (void**)&bucket);
	return bucket->Asset;
}

bool OPcman::Delete(const OPchar* assetKey) {
	OPasset* bucket = NULL;
	hashmap.Get(assetKey, (void**)&bucket);
	purgeList.InsertLast(bucket);
	bucket->Dirty = 1;
	return false;
}

void OPcman::Destroy() {
	OPhashMapBucket* bucket;
	OPuint i, j, n, m;
	OPhashMapPair *pair;

	n = hashmap.count;
	bucket = hashmap.buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		j = 0;
		while (j < m) {
			// mark asset for removal
			Delete(pair->key);
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	// Unload all of the assets
	Purge();

	assetLoaders.Destroy();
	resourceFiles.Destroy();
	hashmap.Destroy();
	purgeList.Destroy();
	OPfree(rootFolder);
}

bool OPcman::SetDir(OPchar* dir) {
	return false;
}

void OPcman::LoadResourcePack(const OPchar* filename) {
	// Grab the next OPresourceFile slot available
	OPresourceFile* resource = OPNEW(OPresourceFile());
	resourceFiles.Push((ui8*)resource);

	// Opens the File handle
	// The file handle will stay open until the resourceFile is unloaded
	resource->resourceFile.Init(filename);

	// Get the resourceFile version
	ui8 version = resource->resourceFile.Readui8();
	OPlogDebug("Version %d", version);

	// The number of resources in this pack
	resource->resourceCount = resource->resourceFile.Readui16();

	// The total length of all names
	// This is used to make a contiguous block of OPchar data so that
	// the lookup of resources is faster
	ui32 lengthOfNames = resource->resourceFile.Readui32();

	// Allocate the necessary data
	// TODO: (garrett) allocate this into a single struct so that there's only 1 allocation
	resource->resourceNames = (OPchar**)OPalloc(sizeof(OPchar*)* resource->resourceCount);
	resource->resourceOffset = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);
	resource->resourceSize = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);

	OPlogDebug("Resource Count %d", resource->resourceCount);

	for (ui16 i = 0; i < resource->resourceCount; i++) {
		// TODO: (garrett) Read in the string into a contiguous block of OPchar data
		resource->resourceNames[i] = resource->resourceFile.ReadString();
		resource->resourceOffset[i] = resource->resourceFile.Readui32();
		resource->resourceSize[i] = resource->resourceFile.Readui32();

		OPlogDebug("Resource %s", resource->resourceNames[i]);
	}
}

OPstream* OPcman::GetResource(const OPchar* resourceName) {
	// Loop through all currently loaded resource packs
	for (ui16 i = 0; i < resourceFiles.Size(); i++) {
		OPresourceFile* resource = (OPresourceFile*)resourceFiles.Get(i);
		// Loop through each resource in the pack
		for (ui16 j = 0; j < resource->resourceCount; j++) {
			// Try to find the resource requested
			if (!OPstringEquals(resourceName, resource->resourceNames[j])) {
				continue;
			}

			// The resource was found in this pack
			// Set the seek position into the file
			// Then load it into an OPstream
			ui32 offset = resource->resourceOffset[j];
			ui32 size = resource->resourceSize[j];

			// TODO: (garrett) This should be done in a way that will support threading
			resource->resourceFile.Seek(offset);
			OPstream* stream = resource->resourceFile.Read(size);
			stream->Source = resource->resourceNames[j];

			return stream;
		}
	}
	return NULL;
}



//
//OPhashMap OP_CMAN_HASHMAP;
//OPassetLoader* OP_CMAN_ASSETLOADERS;
//OPint OP_CMAN_ASSET_LOADER_COUNT;
//OPchar* OP_CMAN_ASSET_FOLDER;
//
//OPuint OP_CMAN_RESOURCE_FILE_COUNT = 0;
//OPresourceFile OP_CMAN_RESOURCE_FILES[OP_CMAN_MAX_RESOURCE_FILES];
//
//OPlinkedList* OP_CMAN_PURGE;
//
//OPlist* _OP_CMAN_ASSETLOADERS = NULL;

//#if defined(_DEBUG)
//#ifdef OPIFEX_WINDOWS
//
//#endif
//#include "./Data/include/OPstring.h"
//i64 OP_CMAN_LAST_CHECKED = 100;
//
//#endif
//
//OPint OPcmanSetDir(OPchar* dir) {
//	OPint result;
//#if defined(OPIFEX_WINDOWS)
//	result = SetCurrentDirectory(dir);
//#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
//	result = chdir(dir);
//#else
//	result = chdir(dir);
//#endif
//	return result;
//}
//
//void OPcmanUpdate(struct OPtimer* timer) {
//#if defined(_DEBUG)
//	i32 i, j;
//	ui64 change;
//	Bucket bucket;
//	OPasset* asset;
//	OP_CMAN_LAST_CHECKED -= timer->Elapsed;
//	if (OP_CMAN_LAST_CHECKED < 0) {
//		// Only check for file changes once per second
//		OP_CMAN_LAST_CHECKED = 100;
//		for (i = 0; i < OP_CMAN_HASHMAP.count; i++) {
//			bucket = OP_CMAN_HASHMAP.buckets[i];
//			for (j = 0; j < bucket.count; j++) {
//				asset = (OPasset*)bucket.pairs[j].value;
//				if (asset != NULL && asset->Reload != NULL) { // Only check the file, if there's a reload function
//					change = OPfile::LastChange(asset->AbsolutePath);
//					if (change != asset->LastChange) {
//						// OPlg("$, %s",asset->FullPath);
//						OPstream* str = OPfile::ReadFromFile(asset->FullPath, 1024);
//						if (asset->Reload(str, &asset->Asset)) {
//							asset->LastChange = change;
//						}
//						str->Destroy();
//					}
//				}
//			}
//		}
//	}
//#endif
//}
//
//void OPcmanAddLoader(OPassetLoader* loader) {
//	if (_OP_CMAN_ASSETLOADERS == NULL) {
//		_OP_CMAN_ASSETLOADERS = OPlist::Create(16, sizeof(OPassetLoader));
//	}
//	_OP_CMAN_ASSETLOADERS->Push((ui8*)loader);
//}
//
//// ______                _   _
////|  ____|              | | (_)
////| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
////|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
////| |  | |_| | | | | (__| |_| | (_) | | | \__ \
////|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
////
//// Specifies how assets will be loaded for each file type
////OPint OPcmanInit(const OPchar* dir){
////	TIMED_BLOCK
////
////	OPint result;
////	OPint i;
////	OPassetLoader* loader;
////
////	result = 0;
////	i = 0;
////
////
////	if (_OP_CMAN_ASSETLOADERS == NULL) {
////		OPlog("\n\n!!! NO ASSET LOADERS HAVE BEEN ADDED\n\n");
////		_OP_CMAN_ASSETLOADERS = OPlist::Create(16, sizeof(OPassetLoader));
////	}
////
////	if (dir) {
////		OP_CMAN_ASSET_FOLDER = OPstringCopy(dir);
////	} else {
////		OP_CMAN_ASSET_FOLDER = OPstringCreateMerged(OPEXECUTABLE_PATH, "assets/"); // OPstringCopy("assets/");
////	}
////
////	OP_CMAN_ASSET_LOADER_COUNT = _OP_CMAN_ASSETLOADERS->Size();
////	OP_CMAN_ASSETLOADERS = (OPassetLoader*)OPalloc(sizeof(OPassetLoader)* OP_CMAN_ASSET_LOADER_COUNT);
////
////	for (i = 0; i < OP_CMAN_ASSET_LOADER_COUNT; i++) {
////		loader = (OPassetLoader*)_OP_CMAN_ASSETLOADERS->Get(i);
////		OPmemcpy(&OP_CMAN_ASSETLOADERS[i], loader, sizeof(OPassetLoader));
////	}
////
////	_OP_CMAN_ASSETLOADERS->Destroy();
////	OPfree(_OP_CMAN_ASSETLOADERS);
////	_OP_CMAN_ASSETLOADERS = NULL;
////
////	// Switch to the assets directory
////#if defined(OPIFEX_WINDOWS)
////	result = SetCurrentDirectory(OP_CMAN_ASSET_FOLDER);
////
////	if (result) {
////		OPlogInfo("Directory Changed: %s", OP_CMAN_ASSET_FOLDER);
////	}
////	else {
////		OPlogErr("!Directory Change Failed: %s", OP_CMAN_ASSET_FOLDER);
////	}
////#else
////	result = chdir(OP_CMAN_ASSET_FOLDER);
////
////	if (result == 0) {
////		OPlog("Directory Changed to: %s", OP_CMAN_ASSET_FOLDER);
////	}
////	else {
////		OPlog("!Directory Change Failed: %s", OP_CMAN_ASSET_FOLDER);
////	}
////#endif
////
////
////#ifdef _DEBUG
////	//OPfree(OP_CMAN_ASSET_FOLDER);
////	//OP_CMAN_ASSET_FOLDER = OPdirCurrent();
////#endif
////
////
////	// create and copy the hashmap
////	OP_CMAN_HASHMAP.Init(OP_CMAN_CAP);
////
////	// create and copy the purge list
////	OP_CMAN_PURGE = OPlinkedList::Create();
////
////	return 1;
////}
//
//// checks to see if an asset is loaded, triggers the load or unload.
//OPint OPCMAN.IsLoaded(const OPchar* key){
//	return OP_CMAN_HASHMAP.Exists(key);
//}
//
//OPint OPCMAN.Load(const OPchar* key){
//
//	const OPchar* ext = NULL;
//	OPint success = 0;
//	if(OP_CMAN_HASHMAP.Exists(key)){
//		OPasset* existing = NULL;
//		OP_CMAN_HASHMAP.Get(key, (void**)&existing);
//
//		// the key may have been added, but see if there
//		// is any valid pointer in the bucket before declaring it exists
//		if(existing){
//			existing->Dirty = 0;
//			return OP_CMAN_KEY_EXISTS;
//		}
//	}
//
//
//
//	ext = strrchr(key, '.');
//	if(ext){
//		OPint i = 0, extLen = (OPuint)strlen(ext);
//		extLen = extLen <= 8 ? extLen : 8;
//
//		for (i = OP_CMAN_ASSET_LOADER_COUNT; i--;){
//			if(OPmemcmp(OP_CMAN_ASSETLOADERS[i].Extension, ext, extLen) == 0){
//				OPasset* assetBucket = NULL;
//				OPchar* fullPath = NULL;
//				void* asset = NULL;
//				OPint len = 0;
//				// the correct loader for this file type has been found
//				OPassetLoader loader = OP_CMAN_ASSETLOADERS[i];
//
//				// build the path string
//				len = (OPuint)strlen(loader.AssetTypePath) + (OPuint)strlen(key);
//				fullPath = (OPchar*)OPalloc(sizeof(OPchar) * len + 1);
//				fullPath[0] = NULL;
//				if(!fullPath) return OP_CMAN_PATH_ALLOC_FAILED;
//                #ifdef OPIFEX_WINDOWS
//    				strcat_s(fullPath, len + 1, loader.AssetTypePath);
//    				strcat_s(fullPath, len + 1, key);
//                #else
//    				fullPath = strcat(fullPath, loader.AssetTypePath);
//    				fullPath = strcat(fullPath, key);
//                #endif
//
//				// load the asset
//				OPstream* str = OPCMAN.GetResource(fullPath);
//				if (str == NULL) {
//					str = OPfile::ReadFromFile(fullPath, 1024);
//				}
//				success = loader.Load(str, &asset);
//				str->Destroy();
//				if(success <= 0) {
//					OPlog("Failed to load %s", fullPath);
//					OPfree(fullPath);
//					return OP_CMAN_ASSET_LOAD_FAILED;
//				}
//
//
//
//				// create the asset to insert into the hashmap
//				if (!(assetBucket = (OPasset*)OPalloc(sizeof(OPasset)))) {
//
//					OPlog("Failed to allocate bucket for %s", fullPath);
//
//					// clean up the string
//					OPfree(fullPath);
//
//					return OP_CMAN_BUCKET_ALLOC_FAILED;
//				}
//				assetBucket->Asset = asset;
//				assetBucket->Unload = loader.Unload;
//				assetBucket->Dirty = 0;
//#if defined(_DEBUG)
//				assetBucket->Reload = loader.Reload;
//				assetBucket->FullPath = fullPath;
//				assetBucket->AbsolutePath = OPstringCreateMerged(OP_CMAN_ASSET_FOLDER, fullPath);
//				assetBucket->LastChange = OPfile::LastChange(assetBucket->AbsolutePath);
//#endif
//
//				// finally insert into the hashmap
//				if(OP_CMAN_HASHMAP.Put(key, assetBucket))
//					return 1;
//
//				return OP_CMAN_INSERTION_FAILED;
//			}
//		}
//		return OP_CMAN_NO_MATCHING_UPLOADER;
//	}
//
//	return OP_CMAN_EXT_NOT_MATCHED;
//}
//OPint OPcmanUnload(const OPchar* key){
//	void* value = NULL;
//	OPasset* asset = NULL;
//
//	if(!OP_CMAN_HASHMAP.Exists(key))
//		return OP_CMAN_KEY_NOT_FOUND;
//
//	if(!OP_CMAN_HASHMAP.Get(key, &value))
//		return OP_CMAN_RETRIEVE_FAILED;
//	asset = (OPasset*)value;
//
//	if(!value) return OP_CMAN_ASSET_LOAD_FAILED;
//
//	if(!asset->Unload(asset->Asset)) return 0;
//	OP_CMAN_HASHMAP.Put(key, NULL);
//	OPfree(asset); asset = NULL;
//
//	return 1;
//}
//
//// Returns a pointer to the asset requested by file name
//void* OPCMAN.Get(const OPchar* key){
//	OPasset* bucket = NULL;
//	if(!OP_CMAN_HASHMAP.Exists(key)) {
//		OPlog("Asset has not been loaded. '%s'", key);
//		return NULL;
//	}
//	OP_CMAN_HASHMAP.Get(key, (void**)&bucket);
//	return bucket->Asset;
//}
//
//OPint OPCMAN.Delete(const OPchar* key){
//	OPasset* bucket = NULL;
//	OP_CMAN_HASHMAP.Get(key, (void**)&bucket);
//	OP_CMAN_PURGE->InsertLast(bucket);
//	return bucket->Dirty = 1;
//}
//
//OPint OPCMAN.Purge(){
//	OPlinkedListNode* n = OP_CMAN_PURGE->First;
//	while (n){
//		OPasset* bucket = (OPasset*)n->Data;
//		OPlinkedListNode* next = n->Next;
//		if(bucket->Dirty){
//			if(!bucket->Unload(bucket->Asset))
//				return 0;
//#if defined(_DEBUG)
//			OPfree(bucket->AbsolutePath);
//			OPfree(bucket->FullPath);
//#endif
//			OPfree(bucket);
//			bucket = NULL;
//		}
//		OP_CMAN_PURGE->Remove(n);
//		n = next;
//	}
//	return 1;
//}
//
//void* OPCMAN.LoadGet(const OPchar* key) {
//	OPint result;
//	result = OPCMAN.Load(key);
//	if (!result) {
//		return NULL;
//	}
//	return OPCMAN.Get(key);
//}
//
//void OPcmanDestroy() {
//
//	Bucket* bucket;
//	OPuint i, j, n, m;
//	KeyValuePair *pair;
//
//	n = OP_CMAN_HASHMAP.count;
//	bucket = OP_CMAN_HASHMAP.buckets;
//	i = 0;
//	while (i < n) {
//		m = bucket->count;
//		pair = bucket->pairs;
//		j = 0;
//		while(j < m) {
//			OPCMAN.Delete(pair->key);
//			pair++;
//			j++;
//		}
//		bucket++;
//		i++;
//	}
//	OPCMAN.Purge();
//
//	if (_OP_CMAN_ASSETLOADERS) {
//		_OP_CMAN_ASSETLOADERS->Destroy();
//		OPfree(_OP_CMAN_ASSETLOADERS);
//	}
//
//	OPfree(OP_CMAN_ASSET_FOLDER);
//	OPfree(OP_CMAN_ASSETLOADERS);
//	OP_CMAN_HASHMAP.Destroy();
//	if (OP_CMAN_PURGE) {
//		OP_CMAN_PURGE->Destroy();
//		OPfree(OP_CMAN_PURGE);
//	}
//}
//
//void OPCMAN.LoadResourcePack(const OPchar* filename) {
//	// Grab the next OPresourceFile slot available
//	OPresourceFile* resource = &OP_CMAN_RESOURCE_FILES[OP_CMAN_RESOURCE_FILE_COUNT++];
//
//	// Opens the File handle
//	// The file handle will stay open until the resourceFile is unloaded
//	resource->resourceFile.Init(filename);
//
//	// Get the resourceFile version
//	ui8 version = resource->resourceFile.Readui8();
//	OPlogDebug("Version %d", version);
//
//	// The number of resources in this pack
//	resource->resourceCount = resource->resourceFile.Readui16();
//
//	// The total length of all names
//	// This is used to make a contiguous block of OPchar data so that
//	// the lookup of resources is faster
//	ui32 lengthOfNames = resource->resourceFile.Readui32();
//
//	// Allocate the necessary data
//	// TODO: (garrett) allocate this into a single struct so that there's only 1 allocation
//	resource->resourceNames = (OPchar**)OPalloc(sizeof(OPchar*)* resource->resourceCount);
//	resource->resourceOffset = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);
//	resource->resourceSize = (ui32*)OPalloc(sizeof(ui32)* resource->resourceCount);
//
//	OPlogDebug("Resource Count %d", resource->resourceCount);
//
//	for (ui16 i = 0; i < resource->resourceCount; i++) {
//		// TODO: (garrett) Read in the string into a contiguous block of OPchar data
//		resource->resourceNames[i] = resource->resourceFile.ReadString();
//		resource->resourceOffset[i] = resource->resourceFile.Readui32();
//		resource->resourceSize[i] = resource->resourceFile.Readui32();
//
//		OPlogDebug("Resource %s", resource->resourceNames[i]);
//	}
//}
//
//OPstream* OPCMAN.GetResource(const OPchar* resourceName) {
//	// Loop through all currently loaded resource packs
//	for (ui16 i = 0; i < OP_CMAN_RESOURCE_FILE_COUNT; i++) {
//		OPresourceFile resource = OP_CMAN_RESOURCE_FILES[i];
//		// Loop through each resource in the pack
//		for (ui16 j = 0; j < resource.resourceCount; j++) {
//			// Try to find the resource requested
//			if (!OPstringEquals(resourceName, resource.resourceNames[j])) {
//				continue;
//			}
//
//			// The resource was found in this pack
//			// Set the seek position into the file
//			// Then load it into an OPstream
//			ui32 offset = resource.resourceOffset[j];
//			ui32 size = resource.resourceSize[j];
//
//			// TODO: (garrett) This should be done in a way that will support threading
//			resource.resourceFile.Seek(offset);
//			OPstream* stream = resource.resourceFile.Read(size);
//			stream->Source = resource.resourceNames[j];
//
//			return stream;
//		}
//	}
//	return NULL;
//}
