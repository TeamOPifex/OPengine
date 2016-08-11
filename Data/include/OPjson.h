#pragma once

struct OPjson;
typedef struct OPjson OPjson;

#include "./Data/include/OPjsonType.h"
#include "./Data/include/OPstream.h"
#include <jansson.h>

struct OPjson {
	json_t* _json;
	
	ui32 Elements();
	ui32 ArraySize();
	OPjson Get(const OPchar* key);
	OPjson ArrayGet(ui32 index);
	i64 I64();
	f32 F32();
	i8 Bool();
	const OPchar* String();
	OPjsonType Type();
	void Log();
	void Destroy();

	inline const OPchar* String(const OPchar* key) {
		return Get(key).String();
	}

	static OPjson Load(const OPchar* data);
};

OPint OPjsonCmanLoad(OPstream* str, void** asset);
OPint OPjsonCmanUnload(void* asset);