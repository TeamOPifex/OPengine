// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

void OPATTRWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;


    Handle<Object> attrs = JS_NEW_OBJECT();
    JS_SET_NUMBER(attrs, "POSITION", (double)OPattributes::POSITION);
    JS_SET_NUMBER(attrs, "UV", (double)OPattributes::UV);
    JS_SET_NUMBER(attrs, "TANGENT", (double)OPattributes::TANGENT);
    JS_SET_NUMBER(attrs, "NORMAL", (double)OPattributes::NORMAL);
    JS_SET_NUMBER(attrs, "COLOR", (double)OPattributes::COLOR);
    JS_SET_NUMBER(attrs, "BONES", (double)OPattributes::BONES);
    JS_SET_NUMBER(attrs, "COLOR4", (double)OPattributes::COLOR4);
    JS_SET_OBJECT(exports, "ATTR", attrs);
}

#endif
