#ifndef OPENGINE_DATA_JSON
#define OPENGINE_DATA_JSON

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPstream.h"
#include <jansson.h>

struct OPjson;
typedef struct OPjson OPjson;

enum OPJSON_TYPE {
	OPJSON_OBJECT,
	OPJSON_ARRAY,
	OPJSON_STRING,
	OPJSON_INTEGER,
	OPJSON_REAL,
	OPJSON_TRUE,
	OPJSON_FALSE,
	OPJSON_NULL,
	OPJSON_UNKNOWN
};

OPjson OPjsonLoad(const OPchar* data);
ui32 OPjsonElements(OPjson json);
ui32 OPjsonArraySize(OPjson json);
OPjson OPjsonGet(OPjson json, const OPchar* key);
OPjson OPjsonArrayGet(OPjson json, ui32 index);

const OPchar* OPjsonString(OPjson json);
i64 OPjsonI64(OPjson json);
f32 OPjsonF32(OPjson json);
i8 OPjsonBool(OPjson json);

enum OPJSON_TYPE OPjsonType(OPjson json);
void OPjsonLog(OPjson root);

void OPjsonDestroy(OPjson json);

OPint OPjsonCmanLoad(OPstream* str, void** asset);

inline const OPchar* OPjsonString(OPjson json, const OPchar* key);

struct OPjson {
	json_t* _json;

	OPjson Get(const OPchar* key) {
		return OPjsonGet(*this, key);
	}

	const OPchar* String() {
		return OPjsonString(*this);
	}

	const OPchar* String(const OPchar* key) {
		return OPjsonString(*this, key);
	}
};

inline const OPchar* OPjsonString(OPjson json, const OPchar* key) {
	return OPjsonString(OPjsonGet(json, key));
}
#endif
