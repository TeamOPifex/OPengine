#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperPipeline.h"

#include "./Pipeline/Pipeline.h"

void _OPloadersAddDefault(const FunctionCallbackInfo<Value>& args) {
    OPlog("load default loaders");
    OPloadersAddDefault();
}

void OPscriptNodeWrapperPipeline(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> loaders = Object::New(isolate);
    NODE_SET_METHOD(loaders, "AddDefault", _OPloadersAddDefault);
    exports->Set(String::NewFromUtf8(isolate, "loaders"), loaders);
}

#endif