// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/Pipeline.h"
#include "./Scripting/include/OPloaderOPS.h"

JS_RETURN_VAL _OPloadersAddDefault(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPloadersAddDefault();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPloadersAddOPS(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPscriptAddLoader();

    JS_RETURN_NULL;
}

void OPloaderWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> loaders = JS_NEW_OBJECT();
    JS_SET_METHOD(loaders, "AddDefault", _OPloadersAddDefault);
    JS_SET_METHOD(loaders, "AddOPS", _OPloadersAddOPS);
    JS_SET_OBJECT(exports, "loaders", loaders);
}
#endif