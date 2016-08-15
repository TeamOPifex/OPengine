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

static OPhashMapPair* get_pair(OPhashMapBucket *bucket, const OPchar *key);
static ui64 hash(const OPchar* str);

// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//


void OPhashMap::Init(OPuint capacity)
{
	count = capacity;
	buckets = (OPhashMapBucket*)OPallocZero(count * sizeof(OPhashMapBucket));
}

void OPhashMap::Destroy()
{
	OPuint i, j, n, m;
	OPhashMapBucket *bucket;
	OPhashMapPair *pair;

	n = count;
	bucket = buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		if (pair) {
			j = 0;
			while (j < m) {
				OPfree(pair->key);
				pair++;
				j++;
			}
			OPfree(bucket->pairs);
		}
		bucket++;
		i++;
	}
	OPfree(buckets);
}

bool OPhashMap::Get(const OPchar* key, void** dest)
{
	OPuint index;
	OPhashMapBucket *bucket;
	OPhashMapPair *pair;

	if (key == NULL) return false;

	index = hash(key) % count;
	bucket = &(buckets[index]);

	pair = get_pair(bucket, key);
	if (pair == NULL) return false;

	(*dest) = pair->value;

	return true;
}

bool OPhashMap::Exists(const OPchar *key)
{
	OPuint index;
	OPhashMapBucket* bucket;
	OPhashMapPair* pair;

	if (key == NULL) return false;

	index = hash(key) % count;

	bucket = &(buckets[index]);

	pair = get_pair(bucket, key);

	if (pair == NULL) return false;

	return true;
}

bool OPhashMap::Remove(const OPchar* key) {
	OPuint index;
	OPhashMapBucket* bucket;
	OPhashMapPair* pair;

	if (key == NULL) return false;

	index = hash(key) % count;

	bucket = &(buckets[index]);

	pair = get_pair(bucket, key);

	if (pair == NULL) return false;

	pair->key = NULL;
	pair->value = NULL;

	return true;
}

OPint OPhashMap::Put(const OPchar* key, void* value)
{
	OPuint key_len, index;
	OPhashMapBucket* bucket;
	OPhashMapPair* tmp_pairs, *pair;

	if (key == NULL) return 0;

	key_len = (OPuint)strlen(key);

	// Get the bucket the key points to
	index = hash(key) % count;
	bucket = &(buckets[index]);

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
		bucket->pairs = OPNEW(OPhashMapPair());
		if (bucket->pairs == NULL) {
			//OPfree(new_key);
			return 0;
		}
		bucket->count = 1;
	}
	else {
		// Add to the array of pairs
		tmp_pairs = (OPhashMapPair*)OPrealloc(bucket->pairs, (bucket->count + 1) * sizeof(OPhashMapPair));
		if (tmp_pairs == NULL) {
			//OPfree(new_key);
			return 0;
		}
		bucket->pairs = tmp_pairs;
		bucket->count++;
	}

	pair = &(bucket->pairs[bucket->count - 1]);
	pair->key = OPstringCopy(key);
	pair->value = value;

	/* Copy the key into the key-value pair */
	//strcpy(pair->key, key);

	return 1;
}

OPint OPhashMap::Count()
{
	OPuint i, j, n, m, items;
	OPhashMapBucket *bucket;
	OPhashMapPair *pair;

	bucket = buckets;
	n = count;
	i = 0;
	items = 0;
	while (i < n) {
		pair = bucket->pairs;
		m = bucket->count;
		j = 0;
		while (j < m) {
			items++;
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	return items;
}

//    _____      _            _         ______                _   _
//   |  __ \    (_)          | |       |  ____|              | | (_)
//   | |__) | __ ___   ____ _| |_ ___  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//   |  ___/ '__| \ \ / / _` | __/ _ \ |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   | |   | |  | |\ V / (_| | ||  __/ | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//   |_|   |_|  |_| \_/ \__,_|\__\___| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
//
static OPhashMapPair* get_pair(OPhashMapBucket *bucket, const OPchar *key)
{
	OPuint i, n;
	OPhashMapPair *pair;

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
