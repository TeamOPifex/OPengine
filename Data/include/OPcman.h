#pragma once

struct OPcman;
typedef struct OPcman OPcman;

#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPvector.h"
#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPassetLoader.h"
#include "./Data/include/OPasset.h"
#include "./Core/include/OPtimer.h"

#define OP_CMAN_CAP 1000

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
	void* LoadFromFile(const OPchar* path);
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