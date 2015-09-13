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

JS_RETURN_VAL _OPphysXControllerCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXControllerManager* manager = JS_GET_ARG_PTR(args, 0, OPphysXControllerManager);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
    OPfloat height = args[2]->NumberValue();
    OPfloat radius = args[3]->NumberValue();
	OPphysXController* ptr = OPphysXControllerCreate(manager, material, height, radius);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	Handle<Object> actor = JS_NEW_OBJECT();
	JS_SET_PTR(actor, ptr->getActor());
	JS_SET_OBJECT(result, "actor", actor);

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
	
	OPphysXControllerSetFootPos(controller, OPvec3Create(args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue()));

    JS_RETURN_NULL;
}
#endif

void OPphysXControllerWrapper(Handle<Object> exports) {
#ifdef OPIFEX_OPTION_PHYSX
    SCOPE_AND_ISOLATE;
    Handle<Object> physXController = JS_NEW_OBJECT();
	JS_SET_METHOD(physXController, "CreateManager", _OPphysXControllerCreateManager);
	JS_SET_METHOD(physXController, "Create", _OPphysXControllerCreate);
	JS_SET_METHOD(physXController, "Move", _OPphysXControllerMove);
	JS_SET_METHOD(physXController, "GetPos", _OPphysXControllerGetPos);
	JS_SET_METHOD(physXController, "GetFootPos", _OPphysXControllerGetFootPos);
	JS_SET_METHOD(physXController, "SetFootPos", _OPphysXControllerSetFootPos);
	JS_SET_OBJECT(exports, "physXController", physXController);
#endif
}

#endif