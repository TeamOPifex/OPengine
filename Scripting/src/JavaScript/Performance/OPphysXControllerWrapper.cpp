#include "./Scripting/include/JavaScript/Performance/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)


#ifdef OPIFEX_OPTION_PHYSX
#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXController.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

JS_RETURN_VAL _OPphysXControllerCreateManager(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXScene* scene = JS_GET_ARG_PTR(args, 0, OPphysXScene);
	OPphysXControllerManager* ptr = OPphysXControllerCreateManager(scene);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

void OnShapeHit(PxControllerShapeHit hit) {
    // OPlog("PhysX Wrapper Controller -> Shape Hit :: %s", hit.shape->getName());
}
void OnControllerHit(PxControllersHit hit) {
    OPlog("PhysX Wrapper Controller -> Controller Hit");
}
void OnObstacleHit(PxControllerObstacleHit hit) {
    OPlog("PhysX Wrapper Controller -> Obstacle Hit");
}

JS_RETURN_VAL _OPphysXControllerCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXControllerManager* manager = JS_GET_ARG_PTR(args, 0, OPphysXControllerManager);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
    OPfloat height = (f32)args[2]->NumberValue();
    OPfloat radius = (f32)args[3]->NumberValue();
	OPphysXController* ptr = OPphysXControllerCreate(manager, material, height, radius, OnShapeHit, OnControllerHit, OnObstacleHit, NULL);

    ptr->setContactOffset(0);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	Handle<Object> actor = JS_NEW_OBJECT();
	JS_SET_PTR(actor, ptr->getActor());
	JS_SET_OBJECT(result, "actor", actor);

    Handle<Array> arr = JS_NEW_ARRAY();
    ui32 numShapes = ptr->getActor()->getNbShapes();
    PxShape** shapes = (PxShape**)OPalloc(sizeof(PxShape*) * numShapes);
    ptr->getActor()->getShapes(shapes, numShapes);
    for(ui32 i = 0; i < numShapes; i++) {
        Handle<Object> arrObj = JS_NEW_OBJECT();
        JS_SET_PTR(arrObj, shapes[i]);
        arr->Set(i, arrObj);
    }
    OPfree(shapes);
	JS_SET_OBJECT(result, "shapes", arr);


	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXControllerMove(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* controller = JS_GET_ARG_PTR(args, 0, OPphysXController);
    OPvec3* displacement = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPtimer* timer = JS_GET_ARG_PTR(args, 2, OPtimer);
	OPphysXControllerMove(controller, *displacement, timer);
	JS_RETURN_NULL;
}


JS_RETURN_VAL _OPphysXControllerGetActorSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* ptr = JS_GET_PTR(args.This(), OPphysXController);

    void* actor = ptr->getActor();

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, actor);
    JS_RETURN(result);
}


JS_RETURN_VAL _OPphysXControllerGetActor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* ptr = JS_GET_ARG_PTR(args, 0, OPphysXController);

    void* actor = ptr->getActor();

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, actor);
	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXControllerGetPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* controller = JS_GET_ARG_PTR(args, 0, OPphysXController);
	OPvec3 result = OPphysXControllerGetPos(controller);

	OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
	Handle<Object> obj = JS_NEW_OBJECT();
	OPvec3WrapperSetup(obj, ptr);

	JS_RETURN(obj);
}

JS_RETURN_VAL _OPphysXControllerGetFootPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* controller = JS_GET_ARG_PTR(args, 0, OPphysXController);
	OPvec3 result = OPphysXControllerGetFootPos(controller);

	Handle<Object> obj = JS_NEW_OBJECT();
	JS_SET_NUMBER(obj, "x", result.x);
	JS_SET_NUMBER(obj, "y", result.y);
	JS_SET_NUMBER(obj, "z", result.z);

	JS_RETURN(obj);
}

JS_RETURN_VAL _OPphysXControllerSetFootPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* controller = JS_GET_ARG_PTR(args, 0, OPphysXController);

	OPphysXControllerSetFootPos(controller, OPvec3Create((f32)args[1]->NumberValue(), (f32)args[2]->NumberValue(), (f32)args[3]->NumberValue()));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXControllerRelease(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXController* controller = JS_GET_ARG_PTR(args, 0, OPphysXController);

	OPphysXControllerRelease(controller);

    JS_RETURN_NULL;
}

#endif

void OPphysXControllerWrapper(Handle<Object> exports) {
#ifdef OPIFEX_OPTION_PHYSX
    SCOPE_AND_ISOLATE;
    Handle<Object> physXController = JS_NEW_OBJECT();
	JS_SET_METHOD(physXController, "CreateManager", _OPphysXControllerCreateManager);
	JS_SET_METHOD(physXController, "Create", _OPphysXControllerCreate);
	JS_SET_METHOD(physXController, "Release", _OPphysXControllerRelease);
	JS_SET_METHOD(physXController, "Move", _OPphysXControllerMove);
	JS_SET_METHOD(physXController, "GetActor", _OPphysXControllerGetActor);
	JS_SET_METHOD(physXController, "GetPos", _OPphysXControllerGetPos);
	JS_SET_METHOD(physXController, "GetFootPos", _OPphysXControllerGetFootPos);
	JS_SET_METHOD(physXController, "SetFootPos", _OPphysXControllerSetFootPos);
	JS_SET_OBJECT(exports, "physXController", physXController);
#endif
}

#endif
