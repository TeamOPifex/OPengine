#ifndef OPEngine_Data_ContentMan
#define OPEngine_Data_ContentMan

#ifdef __cplusplus
extern "C" {
#endif

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPtimer.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"

#define OP_CMAN_CAP 100
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

//  _____ _                   _
// / ____| |                 | |
//| (___ | |_ _ __ _   _  ___| |_ ___
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//


struct OPresourceFile {
	struct OPfile resourceFile;
	ui16 headerSize;
	ui16 resourceCount;
	OPchar** resourceNames;
	ui32* resourceOffset;
	ui32* resourceSize;
};
typedef struct OPresourceFile OPresourceFile;

struct OPassetLoader {
	const OPchar Extension[8];
	const OPchar* AssetTypePath;
	OPint AssetSize;
	OPint(*Load)(OPstream* stream, void** assetOut);
	OPint(*Unload)(void* assetIn);
	OPint(*Reload)(OPstream* stream, void** assetOut);
};
typedef struct OPassetLoader OPassetLoader;

struct OPasset {
	void* Asset;
	OPint(*Unload)(void* assetIn);
	OPint Dirty;
#if defined(_DEBUG)
	OPint(*Reload)(OPstream* stream, void** assetOut);
	OPchar* FullPath;
	OPchar* AbsolutePath;
	i64 LastChange;
#endif
};
typedef struct OPasset OPasset;

//  _____ _       _           _
// / ____| |     | |         | |
//| |  __| | ___ | |__   __ _| |___
//| | |_ | |/ _ \| '_ \ / _` | / __|
//| |__| | | (_) | |_) | (_| | \__ \
// \_____|_|\___/|_.__/ \__,_|_|___/
//

extern OPhashMap OP_CMAN_HASHMAP;
extern OPassetLoader* OP_CMAN_ASSETLOADERS;
extern OPint OP_CMAN_ASSET_LOADER_COUNT;
extern OPlinkedList* OP_CMAN_PURGE;
extern OPresourceFile OP_CMAN_RESOURCE_FILES[OP_CMAN_MAX_RESOURCE_FILES];

// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//

/* *Debug Only*
 * Watches the files of loaded resources for changes
 * Looks at Last Write Time for each file once every second
 * When a change happens the Reload method is called
 */
void OPcmanUpdate(struct OPtimer* timer);

/* Adds a loader to be used when OPcmanInit is called
* @param loader Pointer to a defined asset loader
*/
void OPcmanAddLoader(OPassetLoader* loader);

/* Initializes the Content Manager with an array of Asset Loaders
* A custom directoy can be provided otherwise it defaults to assets/
* @param dir Directory to look for assets, if NULL then it will look in the startup directory for a folder named "Assets"
* @return Success Result
*/
OPint OPcmanInit(const OPchar* dir);

/* Unloads all assets that are no longer needed (marked deleted)
* Assets that are no longer needed have been deleted with OPcmanDelete
* @return Success Result
*/
OPint OPcmanPurge();

/* checks to see if an asset is loaded, triggers the load or unload.
* @param key The resource name to find
* @return Success Result
*/
OPint OPcmanIsLoaded(const OPchar* key);

/* Attempts to load an asset
* @param key The resource name to load
* @return Success Result
*/
OPint OPcmanLoad(const OPchar* key);

/*
* Unloads a resource
* @param key The resource that's no longer needed
* @return Success Result
*/
OPint OPcmanUnload(const OPchar* key);

/* Returns a pointer to the resource requested by file name
* @param key The resource name to load
* @return Pointer to the resource requested. NULL if it wasn't found.
*/
void* OPcmanGet(const OPchar* key);

/* Loads a resource and returns a pointer to the resource requested
* @param key The resource name to load
* @return Pointer to the resource requested. NULL if it failed to load or wasn't found.
*/
void* OPcmanLoadGet(const OPchar* key);

/* Marks an asset as ready to delete
* It will only be removed from memory when OPcmanPurge is called
* @param key The resource that's no longer needed
* @return Success Result
*/
OPint OPcmanDelete(const OPchar* key);

/* Marks an asset as ready to delete
* It will only be removed from memory when OPcmanPurge is called
* @param key The resource that's no longer needed
* @return Success Result
*/
void OPcmanDestroy();

OPint OPcmanSetDir(OPchar* dir);


void OPcmanLoadResourceFile(const OPchar* filename);
OPstream* OPcmanGetResource(const OPchar* resourceName);

#ifdef __cplusplus
}
#endif

#endif
