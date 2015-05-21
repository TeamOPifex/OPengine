// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

void OPATTRWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;


    Handle<Object> attrs = JS_NEW_OBJECT();
    JS_SET_NUMBER(attrs, "POSITION", OPATTR_POSITION);
    JS_SET_NUMBER(attrs, "UV", OPATTR_UV);
    JS_SET_NUMBER(attrs, "TANGENT", OPATTR_TANGENT);
    JS_SET_NUMBER(attrs, "NORMAL", OPATTR_NORMAL);
    JS_SET_NUMBER(attrs, "COLOR", OPATTR_COLOR);
    JS_SET_NUMBER(attrs, "BONES", OPATTR_BONES);
    JS_SET_NUMBER(attrs, "COLOR4", OPATTR_COLOR4);
    JS_SET_OBJECT(exports, "ATTR", attrs);
}

#endif