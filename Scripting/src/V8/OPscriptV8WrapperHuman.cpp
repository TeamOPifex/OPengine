#include "./Scripting/include/V8/OPscriptV8WrapperHuman.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/V8/OPscriptV8.h"
#include "./Human/include/Human.h"

static void _OP_render_Clear(const FunctionCallbackInfo<Value>& args) {
	OPrenderClear(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
}

static void _OP_render_Present(const FunctionCallbackInfo<Value>& args) {
	OPrenderPresent();
}


Handle<Object> _vec3Setup(Handle<Object> obj, OPvec3* val) {

	obj->Set(OPscriptV8GetString("Id"), OPscriptV8GetNumber((OPint)val));
	obj->Set(OPscriptV8GetString("Type"), OPscriptV8GetNumber((OPint)OPscript_VEC3));

	// OPscriptV8SetFunction(obj, "X", _OPvec3X);
	// OPscriptV8SetFunction(obj, "Y", _OPvec3Y);
	// OPscriptV8SetFunction(obj, "Z", _OPvec3Z);
	// OPscriptV8SetFunction(obj, "XYZ", _OPvec3XYZ);
	// OPscriptV8SetFunction(obj, "Set", _OPvec3Set);
	// OPscriptV8SetFunction(obj, "Add", _OPvec3Add);
	// OPscriptV8SetFunction(obj, "Sub", _OPvec3Sub);

	return obj;
}

Handle<Object> OPscriptV8Create(OPvec3* val) {
	Handle<Object> obj = Object::New(OPSCRIPTV8_ISOLATE);
	_vec3Setup(obj, val);
	return obj;
}

void OPscriptV8WrapperHuman(Handle<Object> target) {

	// OP.render
	Handle<Object> render = Object::New(OPSCRIPTV8_ISOLATE);
	OPscriptV8SetFunction(render, "Clear", _OP_render_Clear);
	OPscriptV8SetFunction(render, "Present", _OP_render_Present);
	OPscriptV8SetObject(target, "render", render);
	
}

#endif