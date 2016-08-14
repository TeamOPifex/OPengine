// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPstring.h"


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

    // OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    String::Utf8Value str(args[1]->ToString());
    i16 result = OPskeletonGet(skeletonPtr, *str);

    JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPskeletonGetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    // OPlog("Skeleton Get Self");

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    String::Utf8Value str(args[0]->ToString());
    // OPlog("Find bone %s", *str);
    i16 result = OPskeletonGet(skeletonPtr, *str);

    JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPskeletonLocalTranslate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    // OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 0, OPskeleton);
    i16 index = (i16)args[1]->IntegerValue();
    OPvec3 pos = OPskeletonLocalTranslate(skeletonPtr, index);

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = pos;
    Handle<Object> result = JS_NEW_OBJECT();
    OPvec3WrapperSetup(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPskeletonLocalTranslateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    // OPlog("Skeleton Get");

    OPskeleton* skeletonPtr = JS_GET_PTR(args.This(), OPskeleton);
    i16 index = (i16)args[0]->IntegerValue();
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

Handle<Object> OPskeletonWrapperCreate(Handle<Object> result, OPskeleton* skeleton) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, skeleton);
    JS_SET_METHOD(result, "Update", _OPskeletonUpdateSelf);
    JS_SET_METHOD(result, "Get", _OPskeletonGetSelf);
    JS_SET_METHOD(result, "LocalTranslate", _OPskeletonLocalTranslateSelf);
    JS_SET_METHOD(result, "Destroy", _OPskeletonDestroySelf);
    JS_SET_NUMBER(result, "hierarchyCount", skeleton->hierarchyCount);

    Handle<Object> skinned = JS_NEW_OBJECT();
    JS_SET_PTR(skinned, skeleton->skinned);
    JS_SET_OBJECT(result, "skinned", skinned);

	return result;
}

JS_RETURN_VAL _OPskeletonCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> result = JS_NEW_OBJECT();

    Local<Array> arr = Local<Array>::Cast(args[0]);
    OPuint count =  arr->Length();
    i16* hierarchy = (i16*)OPalloc(sizeof(i16) * count);
    OPmat4* pose = (OPmat4*)OPalloc(sizeof(OPmat4) * count);
    OPchar** names = (OPchar**)OPalloc(sizeof(OPchar*) * count);

    for(OPuint i = 0 ; i < count; i++) {
      hierarchy[i] = (i16)arr->Get(i)->NumberValue();
      // OPlog("h: %d", hierarchy[i]);
    }


    Local<Array> arrPose = Local<Array>::Cast(args[1]);
    count =  arrPose->Length();
    for(ui32 i = 0 ; i < count; i++) {
      Local<Object> item = Local<Object>::Cast(arrPose->Get(i));
      pose[i] = *JS_GET_PTR(item, OPmat4);
      // OPmat4Log("Pose", pose[i]);
    }


    Local<Array> arrNames = Local<Array>::Cast(args[2]);
    count =  arrNames->Length();
    // OPlog("Name Count: %d, %d", count, sizeof(OPchar*));
    // OPlog("Pos %p", names);
    for(ui32 i = 0 ; i < count; i++) {
        // OPlog("C: %d", i);
        String::Utf8Value name(arrNames->Get(i)->ToString());
        // OPlog("Name: %s", *name);
        names[i] = OPstringCopy(*name);
    }

    OPskeleton* skeleton = OPskeletonCreate(hierarchy, pose, count, names);
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

    JS_RETURN_NULL;
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
