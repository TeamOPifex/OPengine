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

void OPscriptV8WrapperHuman(Handle<Object> target) {

	// OP.render
	Handle<Object> render = Object::New(OPSCRIPTV8_ISOLATE);
	OPscriptV8SetFunction(render, "Clear", _OP_render_Clear);
	OPscriptV8SetFunction(render, "Present", _OP_render_Present);
	target->Set(String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "render"), render);
	
}

#endif