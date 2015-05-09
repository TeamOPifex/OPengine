#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"

#include "./Core/include/OPlog.h"
#include "./Human/Human.h"

void _OPrenderInit(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderInit();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void _OPrenderClear(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderClear(
        args[0]->NumberValue(),
        args[1]->NumberValue(),
        args[2]->NumberValue());
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void _OPrenderPresent(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderPresent();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> render = Object::New(isolate);
    NODE_SET_METHOD(render, "Init", _OPrenderInit);
    NODE_SET_METHOD(render, "Clear", _OPrenderClear);
    NODE_SET_METHOD(render, "Present", _OPrenderPresent);
    NODE_SET_OBJECT(exports, "render", render);

}

#endif