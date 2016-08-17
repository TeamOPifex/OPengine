#pragma once

struct OPhashMap;
typedef struct OPhashMap OPhashMap;

#include "./Data/include/OPhashMapBucket.h"
#include "./Core/include/OPmemory.h"

struct OPhashMap {
	OPuint count;
	OPhashMapBucket *buckets;

	OPhashMap() { }
	OPhashMap(OPuint capacity) { Init(capacity); }

	void Init(OPuint capacity);

	/* Creates a HashMap (Dictionary)
	* @param capacity The max number of buckets to hold
	* @return The newly create OPhashMap
	*/
	static inline OPhashMap* Create(OPuint capacity) { return OPNEW(OPhashMap(capacity)); }

	/* Destroys an OPhashMap
	* @param The OPhashMap to destroy
	*/
	void Destroy();

	/* Gets a value out of an OPhashMap
	* @param hashMap The OPhashMap to find a value in
	* @param key The key of the value trying to be found
	* @param value Pointer to a pointer for the value to be stored in if successfully found
	* @return Success Result
	*/
	bool Get(const OPchar* key, void** value);

	/* Determines if an OPhashMap contains a value with a key
	* @param map The OPhashMap to find a value in
	* @param key The key of the value trying to be found
	* @return Success Result
	*/
	bool Exists(const OPchar* key);

	/* Puts a value by a key into an OPhashMap. If all buckets have been used it will fail.
	* @param map The OPhashMap to put the key/value into
	* @param key The key of the value to put into the OPhashMap
	* @param value The pointer value to put into the OPhashMap
	* @return Success Result
	*/
	OPint Put(const OPchar* key, void* value);

	bool Remove(const OPchar* key);

	/* Gets the number of buckets being used by an OPhashMap
	* @param map The OPhashMap to find a count
	* @return The number of buckets used
	*/
	OPint Count();
};
