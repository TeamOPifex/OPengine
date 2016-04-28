#include "./Data/include/OPjson.h"
#include "./Core/include/OPlog.h"

OPjson OPjsonLoad(const OPchar* data) {
	json_error_t error;
	OPjson result;
	result._json = json_loads(data, 0, &error);
	if (!result._json) {
		OPlogErr("json error on line %d: %s\n", error.line, error.text);
	}
	return result;
}

enum OPJSON_TYPE OPjsonType(OPjson json) {
	switch (json_typeof(json._json)) {
    case JSON_OBJECT:
        return OPJSON_OBJECT;
    case JSON_ARRAY:
        return OPJSON_ARRAY;
    case JSON_STRING:
        return OPJSON_STRING;
    case JSON_INTEGER:
        return OPJSON_INTEGER;
    case JSON_REAL:
        return OPJSON_REAL;
    case JSON_TRUE:
        return OPJSON_TRUE;
    case JSON_FALSE:
        return OPJSON_FALSE;
    case JSON_NULL:
        return OPJSON_NULL;
    default:
        return OPJSON_UNKNOWN;
    }
}



void print_json_aux(json_t *element, int indent);
void print_json_indent(int indent);
const char *json_plural(int count);
void print_json_object(json_t *element, int indent);
void print_json_array(json_t *element, int indent);
void print_json_string(json_t *element, int indent);
void print_json_integer(json_t *element, int indent);
void print_json_real(json_t *element, int indent);
void print_json_true(json_t *element, int indent);
void print_json_false(json_t *element, int indent);
void print_json_null(json_t *element, int indent);

void OPjsonLog(OPjson root) {
    print_json_aux(root._json, 0);
}

void print_json_aux(json_t *element, int indent) {
    switch (json_typeof(element)) {
    case JSON_OBJECT:
        print_json_object(element, indent);
        break;
    case JSON_ARRAY:
        print_json_array(element, indent);
        break;
    case JSON_STRING:
        print_json_string(element, indent);
        break;
    case JSON_INTEGER:
        print_json_integer(element, indent);
        break;
    case JSON_REAL:
        print_json_real(element, indent);
        break;
    case JSON_TRUE:
        print_json_true(element, indent);
        break;
    case JSON_FALSE:
        print_json_false(element, indent);
        break;
    case JSON_NULL:
        print_json_null(element, indent);
        break;
    default:
        fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(element));
    }
}
void print_json_indent(int indent) {
    // int i;
    //for (i = 0; i < indent; i++) { OPlg(' '); }
}
const char *json_plural(int count) {
    return count == 1 ? "" : "s";
}

ui32 OPjsonElements(OPjson json) {
	return (ui32)json_object_size(json._json);
}
OPjson OPjsonGet(OPjson json, const OPchar* key) {
	OPjson result = {
		json_object_get(json._json, key)
	};
	return result;
}
// const OPchar* OPjsonKey(OPjson* json) {
// 	const OPchar* key;
//
// }
void print_json_object(json_t *element, int indent) {
    i32 size;
    const char *key;
    json_t *value;

    print_json_indent(indent);

    size = (i32)json_object_size(element);
    OPlog("JSON Object of %ld pair%s:\n", size, json_plural(size));

    json_object_foreach(element, key, value) {
        print_json_indent(indent + 2);
        OPlog("JSON Key: \"%s\"\n", key);
        print_json_aux(value, indent + 2);
    }
}

ui32 OPjsonArraySize(OPjson json) {
	return (ui32)json_array_size(json._json);
}
OPjson OPjsonArrayGet(OPjson json, ui32 index) {
	OPjson result = {
		json_array_get(json._json, index)
	};
	return result;
}

void print_json_array(json_t *element, int indent) {
    ui32 i;
    ui32 size = (ui32)json_array_size(element);
    print_json_indent(indent);
    OPlog("JSON Array of %ld element%s:\n", size, json_plural(size));
    for (i = 0; i < size; i++) {
        print_json_aux(json_array_get(element, i), indent + 2);
    }
}
const OPchar* OPjsonString(OPjson json) {
	return json_string_value(json._json);
}
i64 OPjsonI64(OPjson json) {
	return json_integer_value(json._json);
}
f32 OPjsonF32(OPjson json) {
	return (f32)json_real_value(json._json);
}
i8 OPjsonBool(OPjson json) {
	if(json_typeof(json._json) == JSON_TRUE) {
		return 1;
	}
	return 0;
}

void print_json_string(json_t *element, int indent) {
    print_json_indent(indent);
    OPlog("JSON String: %s", json_string_value(element));
    //printf("JSON String: \"%s\"\n", json_string_value(element));
}
void print_json_integer(json_t *element, int indent) {
    print_json_indent(indent);
    OPlog("JSON Integer: \"%" JSON_INTEGER_FORMAT "\"\n", json_integer_value(element));
}
void print_json_real(json_t *element, int indent) {
    print_json_indent(indent);
    OPlog("JSON Real: %f\n", json_real_value(element));
}
void print_json_true(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    OPlog("JSON True\n");
}
void print_json_false(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    OPlog("JSON False\n");
}
void print_json_null(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    OPlog("JSON Null\n");
}


void OPjsonDestroy(OPjson json) {
	json_decref(json._json);
}


OPint OPjsonCmanLoad(OPstream* str, void** asset) {
	OPjson* data = (OPjson*)OPalloc(sizeof(OPjson));

	*data = OPjsonLoad((OPchar*)str->Data);
	if (!data->_json) {
		return 0;
	}
	*asset = data;
	return 1;
}

OPint OPjsonCmanUnload(void* asset) {
	OPjson* data = (OPjson*)asset;
	OPjsonDestroy(*data);
	OPfree(data);
	return 1;
}
