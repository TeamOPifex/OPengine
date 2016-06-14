#include "./Data/include/OPhashMap.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPlog.h"

//  _____ _                   _
// / ____| |                 | |
//| (___ | |_ _ __ _   _  ___| |_ ___
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//

static KeyValuePair* get_pair(Bucket *bucket, const OPchar *key);
static ui64 hash(const OPchar* str);

// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//


void OPhashMapInit(OPhashMap* hashMap, OPuint capacity)
{
	if (hashMap == NULL) return;

	hashMap->count = capacity;
	hashMap->buckets = (Bucket*)OPallocZero(hashMap->count * sizeof(Bucket));
	if (hashMap->buckets == NULL) {
		OPfree(hashMap);
		return;
	}
}

OPhashMap* OPhashMapCreate(OPuint capacity)
{
	OPhashMap* hashMap = (OPhashMap*)OPalloc(sizeof(OPhashMap));
	OPhashMapInit(hashMap, capacity);
	return hashMap;
}

void OPhashMapDestroy(OPhashMap* hashMap)
{
	OPuint i, j, n, m;
	Bucket *bucket;
	KeyValuePair *pair;

	if (hashMap == NULL) return;

	n = hashMap->count;
	bucket = hashMap->buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		if (pair) {
			j = 0;
			while (j < m) {
				OPfree(pair->key);
				//OPfree(pair->value); // Hashmap shouldn't free the value inserted
				pair++;
				j++;
			}
			OPfree(bucket->pairs);
		}
		bucket++;
		i++;
	}
	OPfree(hashMap->buckets);
}

OPint OPhashMapGet(const OPhashMap* hashMap, const OPchar* key, void** dest)
{
	OPuint index;
	Bucket *bucket;
	KeyValuePair *pair;

	if (hashMap == NULL) return 0;
	if (key == NULL) return 0;

	index = hash(key) % hashMap->count;
	bucket = &(hashMap->buckets[index]);

	pair = get_pair(bucket, key);
	if (pair == NULL) return 0;

	(*dest) = pair->value;

	return 1;
}

OPint OPhashMapExists(const OPhashMap *map, const OPchar *key)
{
	OPuint index;
	Bucket* bucket;
	KeyValuePair* pair;

	if (map == NULL) return 0;
	if (key == NULL) return 0;

	index = hash(key) % map->count;


	bucket = &(map->buckets[index]);


	pair = get_pair(bucket, key);


	if (pair == NULL) return 0;

	return 1;
}

OPint OPhashMapPut(OPhashMap *map, const OPchar* key, void* value)
{
	OPuint key_len, index;
	Bucket* bucket;
	KeyValuePair* tmp_pairs, *pair;
	OPchar* new_key;

	if (map == NULL || key == NULL) return 0;

	key_len = (OPuint)strlen(key);

	// Get the bucket the key points to
	index = hash(key) % map->count;
	bucket = &(map->buckets[index]);

	// Replace the value if the key already exists
	if ((pair = get_pair(bucket, key)) != NULL) {
		//OPfree(pair->value); Hash should not free things inserted into it
		pair->value = value;
		return 1;
	}

	// Create the Key & Value
	//new_key = (OPchar*)OPalloc((key_len + 1) * sizeof(OPchar));
	//if (new_key == NULL) return 0;

	if (bucket->count == 0) {
		// Create the first KeyValuePair in the bucket
		bucket->pairs = (KeyValuePair*)OPalloc(sizeof(KeyValuePair));
		if (bucket->pairs == NULL) {
			//OPfree(new_key);
			return 0;
		}
		bucket->count = 1;
	}
	else {
		// Add to the array of pairs
		tmp_pairs = (KeyValuePair*)OPrealloc(bucket->pairs, (bucket->count + 1) * sizeof(KeyValuePair));
		if (tmp_pairs == NULL) {
			//OPfree(new_key);
			return 0;
		}
		bucket->pairs = tmp_pairs;
		bucket->count++;
	}

	pair = &(bucket->pairs[bucket->count - 1]);
	pair->key = OPstringCreateMerged(key, "");
	pair->value = value;

	/* Copy the key into the key-value pair */
	//strcpy(pair->key, key);

	return 1;
}

OPint OPhashMapCount(const OPhashMap *map)
{
	OPuint i, j, n, m;
	OPuint count;
	Bucket *bucket;
	KeyValuePair *pair;

	if (map == NULL) {
		return 0;
	}
	bucket = map->buckets;
	n = map->count;
	i = 0;
	count = 0;
	while (i < n) {
		pair = bucket->pairs;
		m = bucket->count;
		j = 0;
		while (j < m) {
			count++;
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	return count;
}

//    _____      _            _         ______                _   _
//   |  __ \    (_)          | |       |  ____|              | | (_)
//   | |__) | __ ___   ____ _| |_ ___  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//   |  ___/ '__| \ \ / / _` | __/ _ \ |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   | |   | |  | |\ V / (_| | ||  __/ | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//   |_|   |_|  |_| \_/ \__,_|\__\___| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
//
static KeyValuePair * get_pair(Bucket *bucket, const OPchar *key)
{
	OPuint i, n;
	KeyValuePair *pair;

	n = bucket->count;
	if (n == 0)	return NULL;

	pair = bucket->pairs;
	i = 0;
	while (i < n) {
		if (pair->key != NULL && pair->value != NULL)
			if (strcmp(pair->key, key) == 0)
				return pair;

		pair++;
		i++;
	}
	return NULL;
}

// The key 'str' must have a length longer than 4 characters, or behavior is undefined
static ui64 hash(const OPchar* str)
{

	ui64 hash = 0xA1F9B450;
	OPint i = 0;
	ui8 c = str[0];
	do{
		hash = ((c << 5) + hash) + c;
		c = *(++str);
		++i;
	}while (c || i < 4);

	return hash;
}
