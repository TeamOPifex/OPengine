// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

NODE_RETURN_VAL _OPtextureClearActive(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPtextureClearActive();

    NODE_RETURN_NULL;
}

void OPtextureWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> texture = NODE_NEW_OBJECT();
    NODE_SET_METHOD(texture, "ClearActive", _OPtextureClearActive);
    NODE_SET_OBJECT(exports, "texture", texture);

}