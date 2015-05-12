#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"

#include "./Core/include/OPlog.h"

NODE_RETURN_VAL Method(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE
    NODE_RETURN(NODE_NEW_STRING("world"));
}

void OPscriptNodeWrapperMath(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
}

#endif