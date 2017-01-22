// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"


JS_RETURN_VAL _OPskeletonAnimationUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPskeletonAnimation* skeletonAnimationPtr = JS_GET_ARG_PTR(args, 0, OPskeletonAnimation);
    OPtimer* timerPtr = JS_GET_ARG_PTR(args, 1, OPtimer);
    skeletonAnimationPtr->Update(timerPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPskeletonAnimationApply(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPskeletonAnimation* skeletonAnimationPtr = JS_GET_ARG_PTR(args, 0, OPskeletonAnimation);
    OPskeleton* skeletonPtr = JS_GET_ARG_PTR(args, 1, OPskeleton);
    skeletonAnimationPtr->Apply(skeletonPtr);

    JS_RETURN_NULL;
}

void OPskeletonAnimationWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> skeletonAnimation = JS_NEW_OBJECT();
    JS_SET_METHOD(skeletonAnimation, "Update", _OPskeletonAnimationUpdate);
    JS_SET_METHOD(skeletonAnimation, "Apply", _OPskeletonAnimationApply);
    JS_SET_NUMBER(skeletonAnimation, "size", sizeof(OPskeletonAnimation));
    JS_SET_OBJECT(exports, "skeletonAnimation", skeletonAnimation);

}

#endif
