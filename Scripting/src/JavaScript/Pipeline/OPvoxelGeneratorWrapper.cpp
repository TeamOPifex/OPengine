// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Pipeline/include/OPvoxelGenerator.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

JS_RETURN_VAL _OPvoxelGeneratorDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_ARG_PTR(args, 0, OPvoxelGenerator);
    OPvoxelGeneratorDestroy(ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvoxelGeneratorDestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_PTR(args.This(), OPvoxelGenerator);
    OPvoxelGeneratorDestroy(ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvoxelGeneratorBuild(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_ARG_PTR(args, 0, OPvoxelGenerator);
    OPmesh* mesh = OPvoxelGeneratorBuild(ptr);

    Handle<Object> result = JS_NEW_OBJECT();
    OPmeshWrapperCreate(result, mesh);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPvoxelGeneratorBuildSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_PTR(args.This(), OPvoxelGenerator);
    OPmesh* mesh = OPvoxelGeneratorBuild(ptr);

    Handle<Object> result = JS_NEW_OBJECT();
    OPmeshWrapperCreate(result, mesh);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPvoxelGeneratorAdd(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_ARG_PTR(args, 0, OPvoxelGenerator);
    OPvoxels* voxels = JS_GET_ARG_PTR(args, 1, OPvoxels);
    OPlog("Arg count for add %d", args.Length());
    if(args.Length() == 2) {
          OPvoxelGeneratorAdd(ptr, *voxels);
    } else {
      OPvec4* bones = JS_GET_ARG_PTR(args, 2, OPvec4);
      OPvec4* weights = JS_GET_ARG_PTR(args, 3, OPvec4);
      OPvec3* offset = JS_GET_ARG_PTR(args, 4, OPvec3);
      OPfloat scale = args[5]->NumberValue();

      OPvoxelGeneratorAdd(ptr, *voxels, *bones, *weights, *offset, scale);
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvoxelGeneratorAddSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_PTR(args.This(), OPvoxelGenerator);
    OPvoxels* voxels = JS_GET_ARG_PTR(args, 0, OPvoxels);
    OPlog("Arg count for add %d", args.Length());
    if(args.Length() == 1) {
      OPlog("Generator: %p", ptr);
      OPlog("Voxels: %p", voxels);
          OPvoxelGeneratorAdd(ptr, *voxels);
    } else {
      OPvec4* bones = JS_GET_ARG_PTR(args, 1, OPvec4);
      OPvec4* weights = JS_GET_ARG_PTR(args, 2, OPvec4);
      OPvec3* offset = JS_GET_ARG_PTR(args, 3, OPvec3);
      OPfloat scale = args[4]->NumberValue();

      OPvoxelGeneratorAdd(ptr, *voxels, *bones, *weights, *offset, scale);
    }

    JS_RETURN_NULL;
}

void OPvoxelGeneratorWrapperCreate(Handle<Object> result, OPvoxelGenerator* ptr) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, ptr);
    JS_SET_METHOD(result, "Destroy", _OPvoxelGeneratorDestroySelf);
    JS_SET_METHOD(result, "Add", _OPvoxelGeneratorAddSelf);
    JS_SET_METHOD(result, "Build", _OPvoxelGeneratorBuildSelf);
    JS_SET_NUMBER(result, "size", sizeof(OPvoxelGenerator));
}

JS_RETURN_VAL _OPvoxelGeneratorInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = JS_GET_ARG_PTR(args, 0, OPvoxelGenerator);

    OPvoxelGeneratorInit(ptr, args[1]->IntegerValue());

    Handle<Object> result = JS_NEW_OBJECT();
    OPvoxelGeneratorWrapperCreate(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPvoxelGeneratorCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvoxelGenerator* ptr = (OPvoxelGenerator*)OPallocZero(sizeof(OPvoxelGenerator));
    OPvoxelGeneratorInit(ptr, args[0]->IntegerValue());

    Handle<Object> result = JS_NEW_OBJECT();
    OPvoxelGeneratorWrapperCreate(result, ptr);

    JS_RETURN(result);
}

void OPvoxelGeneratorWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Create", _OPvoxelGeneratorCreate);
    JS_SET_METHOD(result, "Destroy", _OPvoxelGeneratorDestroy);
    JS_SET_METHOD(result, "Add", _OPvoxelGeneratorAdd);
    JS_SET_METHOD(result, "Build", _OPvoxelGeneratorBuild);
    JS_SET_NUMBER(result, "size", sizeof(OPvoxelGenerator));
    JS_SET_OBJECT(exports, "voxelGenerator", result);

}

#endif
