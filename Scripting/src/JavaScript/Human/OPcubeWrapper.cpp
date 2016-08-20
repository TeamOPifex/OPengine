// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPcubeCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* mesh = (OPmodel*)OPalloc(sizeof(OPmodel));
    *mesh = OPcubeCreate(OPvec3Create((f32)args[0]->NumberValue(), (f32)args[1]->NumberValue(), (f32)args[2]->NumberValue()));

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, mesh);

    JS_RETURN(result);
}

void OPcubeWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> cube = JS_NEW_OBJECT();
    JS_SET_METHOD(cube, "Create", _OPcubeCreate);
    JS_SET_NUMBER(cube, "size", sizeof(OPmesh));
    JS_SET_OBJECT(exports, "cube", cube);
}

#endif