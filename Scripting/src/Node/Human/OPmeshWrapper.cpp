// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"


NODE_RETURN_VAL _OPmeshCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    *mesh = OPmeshCreate();
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, mesh);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPmeshDestroy(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshDestroy(mesh);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmeshBuild(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

//    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
//
//    OPmeshBuild(
//        ui32 vertSize,
//        ui32 indSize,
//        ui32 vertCount,
//        ui32 indCount,
//        void* vertices,
//        void* indices);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmeshBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshBind(mesh);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmeshRender(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmeshRender();

    NODE_RETURN_NULL;
}

void OPmeshWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> mesh = NODE_NEW_OBJECT();
    NODE_SET_METHOD(mesh, "Create", _OPmeshCreate);
    NODE_SET_METHOD(mesh, "Destroy", _OPmeshDestroy);
    NODE_SET_METHOD(mesh, "Build", _OPmeshBuild);
    NODE_SET_METHOD(mesh, "Bind", _OPmeshBind);
    NODE_SET_METHOD(mesh, "Render", _OPmeshRender);
    NODE_SET_NUMBER(mesh, "size", sizeof(OPmesh));
    NODE_SET_OBJECT(exports, "mesh", mesh);

}