#pragma once

struct OPcman;
typedef struct OPcman OPcman;

#include "./Data/include/OPresourceFile.h"
#include "./Data/include/OPassetLoader.h"
#include "./Data/include/OPasset.h"
#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPvector.h"
#include "./Core/include/OPtimer.h"

#define OP_CMAN_CAP 1000
#define OP_CMAN_MAX_ASSET_LOADERS 20
#define OP_CMAN_MAX_RESOURCE_FILES 10

#define OP_CMAN_KEY_EXISTS           -1
#define OP_CMAN_NO_MATCHING_UPLOADER -2
#define OP_CMAN_EXT_NOT_MATCHED      -3
#define OP_CMAN_PATH_ALLOC_FAILED    -4
#define OP_CMAN_ASSET_LOAD_FAILED    -5
#define OP_CMAN_BUCKET_ALLOC_FAILED  -6
#define OP_CMAN_INSERTION_FAILED     -7
#define OP_CMAN_RETRIEVE_FAILED      -8
#define OP_CMAN_KEY_NOT_FOUND        -9

struct OPcman {
	OPhashMap hashmap;
	OPvector assetLoaders;
	OPlinkedList purgeList;
	OPvector resourceFiles;
	OPchar* rootFolder;

#ifdef _DEBUG
	i64 lastChecked;
#endif

	OPcman() { }
	OPcman(const OPchar* dir) { Init(dir); }
	
	bool Init(const OPchar* dir);
	void AddLoader(OPassetLoader* loader);
	void Update(OPtimer* timer);
	bool Purge();
	bool Add(const OPchar* assetKey, OPasset* asset);
	bool Load(const OPchar* assetKey);
	bool Unload(const OPchar* assetKey);
	void* Get(const OPchar* assetKey);
	bool Delete(const OPchar* assetKey);
	void Destroy();
	bool SetDir(OPchar* dir);
	void LoadResourcePack(const OPchar* filename);
	OPstream* GetResource(const OPchar* resourceName);

	inline bool IsLoaded(const OPchar* asset) {
		return hashmap.Exists(asset);
	}

	inline void* LoadGet(const OPchar* asset) {
		if (Load(asset)) {
			return Get(asset);
		}
		return NULL;
	}
};

extern OPcman OPCMAN;

#ifdef _DEBUG
	#define OPCMAN_UPDATE(timer) OPCMAN.Update(timer);
#else
	#define OPCMAN_UPDATE(timer) 
#endif