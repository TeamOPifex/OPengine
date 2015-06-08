// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)



JS_RETURN_VAL _OPskeletonUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    OPskeletonUpdate(skeletonPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPskeletonUpdateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    OPskeletonUpdate(skeletonPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPskeletonGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    String::Utf8Value str(args[1]->ToString());
    i16 result = OPskeletonGet(skeletonPtr, *str);

    JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPskeletonGetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Skeleton Get Self");

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    String::Utf8Value str(args[0]->ToString());
    OPlog("Find bone %s", *str);
    i16 result = OPskeletonGet(skeletonPtr, *str);

    JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPskeletonLocalTranslate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    i16 index = args[1]->IntegerValue();
    OPvec3 pos = OPskeletonLocalTranslate(skeletonPtr, index);

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = pos;
    Handle<Object> result = JS_NEW_OBJECT();
    OPvec3WrapperSetup(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPskeletonLocalTranslateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    i16 index = args[0]->IntegerValue();
    OPvec3 pos = OPskeletonLocalTranslate(skeletonPtr, index);

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = pos;
    Handle<Object> result = JS_NEW_OBJECT();
    OPvec3WrapperSetup(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPskeletonDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    OPskeletonDestroy(skeletonPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPskeletonDestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    OPskeletonDestroy(skeletonPtr);

    JS_RETURN_NULL;
}

void OPskeletonWrapperCreate(Handle<Object> result, OPskeleton* skeleton) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, skeleton);
    JS_SET_METHOD(result, "Update", _OPskeletonUpdateSelf);
    JS_SET_METHOD(result, "Get", _OPskeletonGetSelf);
    JS_SET_METHOD(result, "LocalTranslate", _OPskeletonLocalTranslateSelf);
    JS_SET_METHOD(result, "Destroy", _OPskeletonDestroySelf);
}

JS_RETURN_VAL _OPskeletonCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPskeleton* skeleton = (OPskeleton*)OPalloc(sizeof(OPskeleton));
    Handle<Object> result = JS_NEW_OBJECT();;
    OPskeletonWrapperCreate(result, skeleton);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPskeletonWrap(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    Handle<Object> result = JS_NEW_OBJECT();;
    OPskeletonWrapperCreate(result, skeletonPtr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPskeletonAddLoader(const JS_ARGS& args) {
    OPskeletonAddLoader();
    OPskeletonAnimationAddLoader();
}

void OPskeletonWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> skeleton = JS_NEW_OBJECT();
    JS_SET_METHOD(skeleton, "Create", _OPskeletonCreate);
    JS_SET_METHOD(skeleton, "Update", _OPskeletonUpdate);
    JS_SET_METHOD(skeleton, "Get", _OPskeletonGet);
    JS_SET_METHOD(skeleton, "Destroy", _OPskeletonDestroy);
    JS_SET_METHOD(skeleton, "Wrap", _OPskeletonWrap);
    JS_SET_METHOD(skeleton, "AddLoader", _OPskeletonAddLoader);
    JS_SET_METHOD(skeleton, "LocalTranslate", _OPskeletonLocalTranslate);
    //JS_SET_METHOD(skeleton, "Copy", _OPskeletonCopy);
    JS_SET_NUMBER(skeleton, "size", sizeof(OPskeleton));
    JS_SET_OBJECT(exports, "skeleton", skeleton);

}

#endif