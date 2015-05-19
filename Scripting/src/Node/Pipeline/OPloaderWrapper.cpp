// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Pipeline/Pipeline.h"

NODE_RETURN_VAL _OPloadersAddDefault(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPloadersAddDefault();

    NODE_RETURN_NULL;
}

void OPloaderWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> loaders = NODE_NEW_OBJECT();
    NODE_SET_METHOD(loaders, "AddDefault", _OPloadersAddDefault);
    NODE_SET_OBJECT(exports, "loaders", loaders);
}