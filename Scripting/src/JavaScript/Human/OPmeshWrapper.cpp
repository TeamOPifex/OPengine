// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"


JS_RETURN_VAL _OPmeshCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    *mesh = OPmeshCreate();
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, mesh);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPmeshDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = JS_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshDestroy(mesh);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmeshBuild(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

//    OPmesh* mesh = JS_GET_ARG_PTR(args, 0, OPmesh);
//
//    OPmeshBuild(
//        ui32 vertSize,
//        ui32 indSize,
//        ui32 vertCount,
//        ui32 indCount,
//        void* vertices,
//        void* indices);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmeshBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = JS_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshBind(mesh);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmeshRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmeshRender();

    JS_RETURN_NULL;
}

void OPmeshWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> mesh = JS_NEW_OBJECT();
    JS_SET_METHOD(mesh, "Create", _OPmeshCreate);
    JS_SET_METHOD(mesh, "Destroy", _OPmeshDestroy);
    JS_SET_METHOD(mesh, "Build", _OPmeshBuild);
    JS_SET_METHOD(mesh, "Bind", _OPmeshBind);
    JS_SET_METHOD(mesh, "Render", _OPmeshRender);
    JS_SET_NUMBER(mesh, "size", sizeof(OPmesh));
    JS_SET_OBJECT(exports, "mesh", mesh);

}

#endif