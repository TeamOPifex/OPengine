#include "./Scripting/include/JavaScript/Performance/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXScene.h"

JS_RETURN_VAL _OPphysXSceneInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    OPvec3* gravity = JS_GET_ARG_PTR(args, 1, OPvec3);
	OPphysXSceneInit(scene, *gravity, NULL, NULL);
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSceneCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    OPvec3* gravity = JS_GET_ARG_PTR(args, 0, OPvec3);

    OPphysXScene* ptr = OPphysXSceneCreate(*gravity);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXSceneCreateDynamic(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    OPvec3* position = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXRigidDynamic* ptr = OPphysXSceneCreateDynamic(scene, *position);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL OPphysXSceneCreateStatic(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    OPvec3* position = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXRigidStatic* ptr = OPphysXSceneCreateStatic(scene, *position);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXSceneUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    OPtimer* timer = JS_GET_ARG_PTR(args, 1, OPtimer);
	OPphysXSceneUpdate(scene, timer);
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSceneAddActor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    PxRigidActor* actor = JS_GET_ARG_PTR(args, 1, PxRigidActor);
	OPphysXSceneAddActor(scene, actor);
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSceneRemove(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 1, OPphysXRigidActor);
	OPphysXSceneRemove(scene, actor);
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSceneDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
	OPphysXSceneDestroy(scene);
	JS_RETURN_NULL;
}

void OPphysXSceneWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> physXScene = JS_NEW_OBJECT();
	JS_SET_METHOD(physXScene, "Init", _OPphysXSceneInit);
	JS_SET_METHOD(physXScene, "Create", _OPphysXSceneCreate);
	JS_SET_METHOD(physXScene, "CreateDynamic", _OPphysXSceneCreateDynamic);
	JS_SET_METHOD(physXScene, "CreateStatic", OPphysXSceneCreateStatic);
	JS_SET_METHOD(physXScene, "Update", _OPphysXSceneUpdate);
	JS_SET_METHOD(physXScene, "AddActor", _OPphysXSceneAddActor);
	JS_SET_METHOD(physXScene, "Remove", _OPphysXSceneRemove);
	JS_SET_METHOD(physXScene, "Destroy", _OPphysXSceneDestroy);
	JS_SET_OBJECT(exports, "physXScene", physXScene);
    
}

#endif