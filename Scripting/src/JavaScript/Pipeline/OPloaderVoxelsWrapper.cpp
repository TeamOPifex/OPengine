// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/Pipeline.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Pipeline/include/Loaders/OPloaderVoxels.h"


void OPvoxelsWrapperCreate(Handle<Object> result, OPvoxels* ptr) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, ptr);

    Handle<Object> size = JS_NEW_OBJECT();
    JS_SET_NUMBER(size, "x", ptr->size.x);
    JS_SET_NUMBER(size, "y", ptr->size.y);
    JS_SET_NUMBER(size, "z", ptr->size.z);
    JS_SET_OBJECT(result, "size", size);

    Handle<Object> offset = JS_NEW_OBJECT();
    JS_SET_NUMBER(offset, "x", ptr->offset.x);
    JS_SET_NUMBER(offset, "y", ptr->offset.y);
    JS_SET_NUMBER(offset, "z", ptr->offset.z);
    JS_SET_OBJECT(result, "offset", offset);

}

JS_RETURN_VAL _OPvoxelsLoad(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    String::Utf8Value asset(args[0]->ToString());

    OPvoxels* ptr = (OPvoxels*)OPcmanLoadGet(*asset);
    
    Handle<Object> result = JS_NEW_OBJECT();
    OPvoxelsWrapperCreate(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPvoxelsGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxels* ptr = JS_GET_ARG_PTR(args, 0, OPvoxels);
    
    OPvecInt3 r = OPvoxelsGet(ptr, args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue());

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_NUMBER(result, "x", r.x);
    JS_SET_NUMBER(result, "y", r.y);
    JS_SET_NUMBER(result, "z", r.z);

    JS_RETURN(result);
}

void OPloaderVoxelsWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Load", _OPvoxelsLoad);
    JS_SET_METHOD(result, "Get", _OPvoxelsGet);
    JS_SET_OBJECT(exports, "voxels", result);
}
#endif
