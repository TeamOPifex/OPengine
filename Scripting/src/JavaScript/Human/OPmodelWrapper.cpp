// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

JS_RETURN_VAL _OPmodelDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_ARG_PTR(args, 0, OPmodel);
    OPfree(model);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelDestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_PTR(args.This(), OPmodel);
    OPfree(model);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_ARG_PTR(args, 0, OPmodel);
    OPmaterial* material = JS_GET_ARG_PTR(args, 1, OPmaterial);
    OPmodelBind(*model, material);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelBindSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    OPmodel* model = JS_GET_PTR(args.This(), OPmodel);
    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPmodelBind(*model, material);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelDraw(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_ARG_PTR(args, 0, OPmodel);
    OPmaterial* material = JS_GET_ARG_PTR(args, 1, OPmaterial);
    OPcam* camera = JS_GET_ARG_PTR(args, 2, OPcam);

    OPmodelDraw(*model, material, camera);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelDrawSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_PTR(args.This(), OPmodel);
    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPcam* camera = JS_GET_ARG_PTR(args, 1, OPcam);

    OPmodelDraw(*model, material, camera);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelSetMesh(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_ARG_PTR(args, 0, OPmodel);
    OPmesh* mesh = JS_GET_ARG_PTR(args, 1, OPmesh);

    model->mesh = mesh;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmodelSetMeshSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmodel* model = JS_GET_PTR(args.This(), OPmodel);
    OPmesh* mesh = JS_GET_ARG_PTR(args, 0, OPmesh);

    model->mesh = mesh;

    JS_RETURN_NULL;
}

Handle<Object> OPmodelWrapperCreate(Handle<Object> result, OPmodel* model) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, model);
    JS_SET_METHOD(result, "Destroy", _OPmodelDestroySelf);
    JS_SET_METHOD(result, "Bind", _OPmodelBindSelf);
    JS_SET_METHOD(result, "SetMesh", _OPmodelSetMeshSelf);
    JS_SET_METHOD(result, "Draw", _OPmodelDrawSelf);
    JS_SET_NUMBER(result, "size", sizeof(OPmodel));

    Handle<Object> world = JS_NEW_OBJECT();
    OPmat4WrapperCreate(world, &((OPmodel*)model)->world);
    JS_SET_OBJECT(result, "world", world);

    return result;
}

JS_RETURN_VAL _OPmodelCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmesh* mesh = JS_GET_ARG_PTR(args, 0, OPmesh);

    OPmodel* model = (OPmodel*)OPallocZero(sizeof(OPmodel));
    model->mesh = mesh;
    model->world = OPMAT4_IDENTITY;

    Handle<Object> result = JS_NEW_OBJECT();
    OPmodelWrapperCreate(result, model);

    JS_RETURN(result);
}

void OPmodelWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> model = JS_NEW_OBJECT();
    JS_SET_METHOD(model, "Create", _OPmodelCreate);
    JS_SET_METHOD(model, "Destroy", _OPmodelDestroy);
    JS_SET_METHOD(model, "SetMesh", _OPmodelSetMesh);
    JS_SET_METHOD(model, "Bind", _OPmodelBind);
    JS_SET_METHOD(model, "Draw", _OPmodelDraw);
    JS_SET_NUMBER(model, "size", sizeof(OPmodel));
    JS_SET_OBJECT(exports, "model", model);

}

Handle<Object> OPmodelTexturedWrapper(Handle<Object> result, OPmodelTextured* model) {
    SCOPE_AND_ISOLATE

    JS_SET_OBJECT_PTR(result, "texture", model->texture);
    return OPmodelWrapperCreate(result, &model->model);
}

#endif
