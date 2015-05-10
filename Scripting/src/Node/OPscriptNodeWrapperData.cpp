#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperData.h"

#include "./Data/Data.h"
#include "./Core/include/OPlog.h"


void _OPcmanInit(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

 	const char* assetDir = NULL;

 	if (args.Length() > 0) {
 		v8::String::Utf8Value utf8(args[0]);
        assetDir = *utf8;
        OPlog("Assets %s", assetDir);
    }

    OPcmanInit(assetDir);
}

void _OPcmanLoad(const FunctionCallbackInfo<Value>& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        OPlog("Script Load1: %s", file);
        OPlog("Test");
        OPint result = OPcmanLoad(file);
        OPlog("Script Load1: %d", result);
        args.GetReturnValue().Set(Number::New(isolate, result));
 	}
 }

void _OPcmanGet(const FunctionCallbackInfo<Value>& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        OPlog("Script Load: %s", file);
        void* result = OPcmanGet(file);
        OPlog("Script Load: %x", result);
        args.GetReturnValue().Set(Number::New(isolate, (OPint)result));
 	}
}

void _OPcmanLoadGet(const FunctionCallbackInfo<Value>& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        OPlog("Script Load: %s", file);
        void* result = OPcmanLoadGet(file);
        OPlog("Script Load: %x", result);
        args.GetReturnValue().Set(Number::New(isolate, (OPint)result));
 	}
 }

void OPscriptNodeWrapperData(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    {
        // OP.cman
        Handle<Object> cman = Object::New(isolate);
        NODE_SET_METHOD(cman, "Init", _OPcmanInit);
        NODE_SET_METHOD(cman, "Load", _OPcmanLoad);
        NODE_SET_METHOD(cman, "Get", _OPcmanGet);
        NODE_SET_METHOD(cman, "LoadGet", _OPcmanLoadGet);
        exports->Set(String::NewFromUtf8(isolate, "cman"), cman);
    }

}

#endif