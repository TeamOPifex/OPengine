#ifndef OPEngine_Data_HashMap
#define OPEngine_Data_HashMap

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
#include "./Core/include/OPtypes.h"

//  _____ _                   _       
// / ____| |                 | |      
//| (___ | |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//                                                                      
typedef struct{
	OPchar* key;
	void* value;
} KeyValuePair;

typedef struct{
	OPuint count;
	KeyValuePair *pairs;
} Bucket;

typedef struct{
	OPuint count;
	Bucket *buckets;
} OPhashMap;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                                                    

/* Creates a HashMap (Dictionary)
 * @param capacity The max number of buckets to hold
 * @return The newly create OPhashMap
*/
OPhashMap* OPhashMapCreate(OPuint capacity);

/* Destroys an OPhashMap
 * @param The OPhashMap to destroy
*/
void OPhashMapDestroy(OPhashMap* map);

/* Gets a value out of an OPhashMap
 * @param hashMap The OPhashMap to find a value in
 * @param key The key of the value trying to be found
 * @param value Pointer to a pointer for the value to be stored in if successfully found
 * @return Success Result
*/
OPint OPhashMapGet(const OPhashMap* hashMap, const OPchar* key, void** value);

/* Determines if an OPhashMap contains a value with a key
 * @param map The OPhashMap to find a value in
 * @param key The key of the value trying to be found
 * @return Success Result
*/
OPint OPhashMapExists(const OPhashMap* map, const OPchar* key);

/* Puts a value by a key into an OPhashMap. If all buckets have been used it will fail.
 * @param map The OPhashMap to put the key/value into
 * @param key The key of the value to put into the OPhashMap
 * @param value The pointer value to put into the OPhashMap
 * @return Success Result
*/
OPint OPhashMapPut(OPhashMap* map, const OPchar* key, void* value);

/* Gets the number of buckets being used by an OPhashMap
 * @param map The OPhashMap to find a count
 * @return The number of buckets used
*/
OPint OPhashMapCount(const OPhashMap* map);

#ifdef __cplusplus
}
#endif

#endif
