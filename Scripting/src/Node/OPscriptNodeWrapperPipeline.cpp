#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperPipeline.h"

#include "./Pipeline/Pipeline.h"

NODE_RETURN_VAL _OPloadersAddDefault(const NODE_ARGS& args) {
    OPlog("load default loaders");
    OPloadersAddDefault();
    OPlog("loaded default loaders");
    NODE_RETURN_NULL
}

void OPscriptNodeWrapperPipeline(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> loaders = NODE_NEW_OBJECT();
    NODE_SET_METHOD(loaders, "AddDefault", _OPloadersAddDefault);
    NODE_SET_OBJECT(exports, "loaders", loaders);
}

#endif