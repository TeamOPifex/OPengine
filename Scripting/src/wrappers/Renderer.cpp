#include "./Scripting/include/wrappers/Renderer.h"

#ifdef OPIFEX_V8

#include "./Core/include/Types.h"
#include "./Human/include/Rendering/Renderer.h"

static void OPscriptRendererClearCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 3) return;
	HandleScope scope(args.GetIsolate());
	Handle<Value> r = args[0];
	Handle<Value> g = args[1];
	Handle<Value> b = args[2];
	f32 rVal = r->Int32Value() / 255.0f;
	f32 gVal = g->Int32Value() / 255.0f;
	f32 bVal = b->Int32Value() / 255.0f;
	OPrenderClear(rVal, gVal, bVal);
}

void OPscriptRendererWrapper(Isolate* isolate, Handle<ObjectTemplate> global) {
	Handle<FunctionTemplate> method = FunctionTemplate::New(isolate, OPscriptRendererClearCallback);
	global->Set(String::NewFromUtf8(isolate, "OPrenderClear"), method);
}

#endif