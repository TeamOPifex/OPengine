// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

NODE_RETURN_VAL _OPcubeCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    *mesh = OPcubeCreate(OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()));

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, mesh);

    NODE_RETURN(result);
}

void OPcubeWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> cube = NODE_NEW_OBJECT();
    NODE_SET_METHOD(cube, "Create", _OPcubeCreate);
    NODE_SET_OBJECT(exports, "cube", cube);
}