// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPtextureClearActive(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPtextureClearActive();

    JS_RETURN_NULL;
}

void OPtextureWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> texture = JS_NEW_OBJECT();
    JS_SET_METHOD(texture, "ClearActive", _OPtextureClearActive);
    JS_SET_OBJECT(exports, "texture", texture);

}

#endif