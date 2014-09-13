#ifndef OPEngine_Data_HashMap
#define OPEngine_Data_HashMap

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
#include "./Core/include/Types.h"

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
} HashMap;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                                                    
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
