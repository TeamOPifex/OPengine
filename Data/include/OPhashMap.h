#pragma once

#ifndef OPEngine_Data_HashMap
#define OPEngine_Data_HashMap

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
#include "./Core/include/Types.h"

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
} HashMap;

HashMap* OPhashMapCreate(OPuint capacity);
void OPhashMapDestroy(HashMap* map);
OPint OPhashMapGet(const HashMap* hashMap, const OPchar* key, void** value);
OPint OPhashMapExists(const HashMap* map, const OPchar* key);
OPint OPhashMapPut(HashMap* map, const OPchar* key, void* value);
OPint OPhashMapCount(const HashMap* map);

#ifdef __cplusplus
}
#endif

#endif
