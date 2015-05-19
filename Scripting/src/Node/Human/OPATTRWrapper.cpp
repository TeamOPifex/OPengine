// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

void OPATTRWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;


    Handle<Object> attrs = NODE_NEW_OBJECT();
    NODE_SET_NUMBER(attrs, "POSITION", OPATTR_POSITION);
    NODE_SET_NUMBER(attrs, "UV", OPATTR_UV);
    NODE_SET_NUMBER(attrs, "TANGENT", OPATTR_TANGENT);
    NODE_SET_NUMBER(attrs, "NORMAL", OPATTR_NORMAL);
    NODE_SET_NUMBER(attrs, "COLOR", OPATTR_COLOR);
    NODE_SET_NUMBER(attrs, "BONES", OPATTR_BONES);
    NODE_SET_NUMBER(attrs, "COLOR4", OPATTR_COLOR4);
    NODE_SET_OBJECT(exports, "ATTR", attrs);
}