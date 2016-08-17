#include "./Data/include/OPjson.h"
#include "./Core/include/OPlog.h"

OPjson OPjson::Load(const OPchar* data) {
	json_error_t error;
	OPjson result;
	result._json = json_loads(data, 0, &error);
	if (!result._json) {
		OPlogErr("json error on line %d: %s\n", error.line, error.text);
	}
	return result;
}

OPjsonType OPjson::Type() {
	switch (json_typeof(_json)) {
    case JSON_OBJECT:
        return OPjsonType::OBJECT;
    case JSON_ARRAY:
        return OPjsonType::ARRAY;
    case JSON_STRING:
        return OPjsonType::STRING;
    case JSON_INTEGER:
        return OPjsonType::INTEGER;
    case JSON_REAL:
        return OPjsonType::REAL;
    case JSON_TRUE:
        return OPjsonType::TRUE_;
    case JSON_FALSE:
        return OPjsonType::FALSE_;
    case JSON_NULL:
        return OPjsonType::NULL_;
    default:
        return OPjsonType::UNKNOWN;
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

void OPjson::Log() {
    print_json_aux(_json, 0);
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

ui32 OPjson::Elements() {
	return (ui32)json_object_size(_json);
}

OPjson OPjson::Get(const OPchar* key) {
	OPjson result = {
		json_object_get(_json, key)
	};
	return result;
}

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

ui32 OPjson::ArraySize() {
	return (ui32)json_array_size(_json);
}

OPjson OPjson::ArrayGet(ui32 index) {
	OPjson result = {
		json_array_get(_json, index)
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
const OPchar* OPjson::String() {
	return json_string_value(_json);
}
i64 OPjson::I64() {
	return json_integer_value(_json);
}
f32 OPjson::F32() {
	return (f32)json_real_value(_json);
}
i8 OPjson::Bool() {
	if(json_typeof(_json) == JSON_TRUE) {
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


void OPjson::Destroy() {
	json_decref(_json);
}


OPint OPjsonCmanLoad(OPstream* str, void** asset) {
	OPjson* data = (OPjson*)OPalloc(sizeof(OPjson));

	*data = OPjson::Load((OPchar*)str->Data);
	if (!data->_json) {
		return 0;
	}
	*asset = data;
	return 1;
}

OPint OPjsonCmanUnload(void* asset) {
	OPjson* data = (OPjson*)asset;
	data->Destroy();
	OPfree(data);
	return 1;
}
