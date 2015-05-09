#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"

#include "./Core/include/OPlog.h"

void Method(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void OPscriptNodeWrapperMath(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
}

#endif