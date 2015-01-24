#include "./Scripting/include/V8/OPscriptV8WrapperMath.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/V8/OPscriptV8.h"
#include "./Math/include/Math.h"

void _OPscriptV8Vec3Update(Handle<Object> obj, OPvec3* val) {
	obj->Set(OPscriptV8GetString("x"), OPscriptV8GetNumber(val->x));
	obj->Set(OPscriptV8GetString("y"), OPscriptV8GetNumber(val->y));
	obj->Set(OPscriptV8GetString("z"), OPscriptV8GetNumber(val->z));
}

static void _OPscriptV8Vec3Set(const FunctionCallbackInfo<Value>& args) {
	Handle<Object> obj = args.This();
	OPvec3* vec = (OPvec3*)(obj->Get(OPscriptV8GetString("Id"))->IntegerValue());
	vec->x = args[0]->NumberValue();
	vec->y = args[1]->NumberValue();
	vec->z = args[2]->NumberValue();
	_OPscriptV8Vec3Update(obj, vec);
}

static void _OPscriptV8Vec3XYZ(const FunctionCallbackInfo<Value>& args) {
	Handle<Object> obj = args.This();
	OPvec3* vec = (OPvec3*)(obj->Get(OPscriptV8GetString("Id"))->IntegerValue());
	Handle<Array> tmp = Array::New(OPSCRIPTV8_ISOLATE);
	tmp->Set(0, OPscriptV8GetNumber(vec->x));
	tmp->Set(1, OPscriptV8GetNumber(vec->y));
	tmp->Set(2, OPscriptV8GetNumber(vec->z));
	args.GetReturnValue().Set(tmp);
}

static void _OPscriptV8Vec3Add(const FunctionCallbackInfo<Value>& args) {
	Handle<Object> obj = args.This();
	OPvec3* vec = (OPvec3*)(obj->Get(OPscriptV8GetString("Id"))->IntegerValue());
	vec->x += args[0]->NumberValue();
	vec->y += args[1]->NumberValue();
	vec->z += args[2]->NumberValue();
	_OPscriptV8Vec3Update(obj, vec);
}

static void _OPscriptV8Vec3Sub(const FunctionCallbackInfo<Value>& args) {
	Handle<Object> obj = args.This();
	OPvec3* vec = (OPvec3*)(obj->Get(OPscriptV8GetString("Id"))->IntegerValue());
	vec->x -= args[0]->NumberValue();
	vec->y -= args[1]->NumberValue();
	vec->z -= args[2]->NumberValue();
	_OPscriptV8Vec3Update(obj, vec);
}

Handle<Object> _OPscriptV8Setup(Handle<Object> obj, OPvec3* val) {

	obj->Set(OPscriptV8GetString("Id"), OPscriptV8GetNumber((OPint)val));
	obj->Set(OPscriptV8GetString("Type"), OPscriptV8GetNumber((OPint)OPscript_VEC3));
	_OPscriptV8Vec3Update(obj, val);

	OPscriptV8SetFunction(obj, "Set", _OPscriptV8Vec3Set);
	OPscriptV8SetFunction(obj, "XYZ", _OPscriptV8Vec3XYZ);
	OPscriptV8SetFunction(obj, "Add", _OPscriptV8Vec3Add);
	OPscriptV8SetFunction(obj, "Sub", _OPscriptV8Vec3Sub);

	return obj;
}

Handle<Object> OPscriptV8Create(OPvec3* val) {
	return _OPscriptV8Setup(Object::New(OPSCRIPTV8_ISOLATE), val);
}

static void _OPscriptV8Vec3Create(const FunctionCallbackInfo<Value>& args) {
	OPvec3* vec = (OPvec3*)OPalloc(sizeof(OPvec3));

	if (args.Length() == 0) {
		*vec = OPVEC3_ZERO;
	} else {
		vec->x = args[0]->NumberValue();
		vec->y = args[1]->NumberValue();
		vec->z = args[2]->NumberValue();
	}

	Handle<Object> obj = OPscriptV8Create(vec);
	args.GetReturnValue().Set(obj);
}

void OPscriptV8WrapperMath(Handle<Object> target) {


	// OP.vec3
	Handle<Object> vec3 = Object::New(OPSCRIPTV8_ISOLATE);
	OPscriptV8SetFunction(vec3, "Create", _OPscriptV8Vec3Create);
	OPscriptV8SetObject(target, "vec3", vec3);
	
}

#endif