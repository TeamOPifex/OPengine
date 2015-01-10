#include "../../include/wrappers/PerformanceWrapper.h"

#ifdef OPIFEX_V8

#include "./Core/include/OPlog.h"
#include "./Performance/include/OPphysics.h"

static V8Return _physicsInit(const V8Args& args);
static V8Return _physicsCreateScene(const V8Args& args);
static V8Return _physicsUpdate(const V8Args& args);
static V8Return _physicsDestroy(const V8Args& args);
static V8Return _physicsShutdown(const V8Args& args);

static V8Return _physicsAddForce(const V8Args& args);
static V8Return _physicsAddTorque(const V8Args& args);
static V8Return _physicsSetLinearVelocity(const V8Args& args);
static V8Return _physicsSetAngularVelocity(const V8Args& args);

static V8Return _physicsGetTransform(const V8Args& args);
static V8Return _physicsCreateBoxDynamic(const V8Args& args);
static V8Return _physicsCreateSphereDynamic(const V8Args& args);
static V8Return _physicsCreateBoxStatic(const V8Args& args);
static V8Return _physicsCreateSphereStatic(const V8Args& args);
static V8Return _physicsCreatePlane(const V8Args& args);

void PerformanceInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	// OP.cman
	V8ObjectGlobal cman = CreateObjectG(isolate);
	SetFunctionG(isolate, cman, "Init", _physicsInit);
	SetFunctionG(isolate, cman, "CreateScene", _physicsCreateScene);
	SetFunctionG(isolate, cman, "Shutdown", _physicsShutdown);
	SetObjectG(isolate, target, "physics", cman);

}

void PerformanceInitializeMethodsO(V8isolate* isolate, V8Object target) {

	// OP.cman
	V8Object cman = CreateObject(isolate);
	SetFunction(isolate, cman, "Init", _physicsInit);
	SetFunction(isolate, cman, "CreateScene", _physicsCreateScene);
	SetFunction(isolate, cman, "Shutdown", _physicsShutdown);
	SetObject(isolate, target, "physics", cman);

}

static V8Return _physicsInit(const V8Args& args) {
	V8Scope scope;

	OPphysicsInit();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsShutdown(const V8Args& args) {
	V8Scope scope;

	OPphysicsShutdown();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsCreateScene(const V8Args& args) {
	V8Scope scope;
	
	OPphysicsScene* scene = OPphysicsCreateScene();

	V8Object obj = CreateTypedObject(isolate, scene, OPscript_PHYSICS_SCENE);
	SetFunction(isolate, obj, "Update", _physicsUpdate);
	SetFunction(isolate, obj, "Destroy", _physicsDestroy);
	SetFunction(isolate, obj, "CreateBoxDynamic", _physicsCreateBoxDynamic);
	SetFunction(isolate, obj, "CreateSphereDynamic", _physicsCreateSphereDynamic);
	SetFunction(isolate, obj, "CreateBoxStatic", _physicsCreateBoxStatic);
	SetFunction(isolate, obj, "CreateSphereStatic", _physicsCreateSphereStatic);
	SetFunction(isolate, obj, "CreatePlane", _physicsCreatePlane);

	return SetReturn(args, &scope, obj);
}

static V8Return _physicsUpdate(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);

	OPphysicsStep(scene, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsDestroy(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);
	OPphysicsDestroy(scene);
	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsAddForce(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPphysicsDynamic* actor = (OPphysicsDynamic*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_DYNAMIC);

	f32 x = args[1 - inScope]->NumberValue();
	f32 y = args[2 - inScope]->NumberValue();
	f32 z = args[3 - inScope]->NumberValue();

	OPphysicsAddForce(actor, x, y, z);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsAddTorque(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPphysicsDynamic* actor = (OPphysicsDynamic*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_DYNAMIC);

	f32 x = args[1 - inScope]->NumberValue();
	f32 y = args[2 - inScope]->NumberValue();
	f32 z = args[3 - inScope]->NumberValue();

	OPphysicsAddTorque(actor, x, y, z);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsSetLinearVelocity(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPphysicsDynamic* actor = (OPphysicsDynamic*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_DYNAMIC);

	f32 x = args[1 - inScope]->NumberValue();
	f32 y = args[2 - inScope]->NumberValue();
	f32 z = args[3 - inScope]->NumberValue();

	OPphysicsSetLinearVelocity(actor, x, y, z);

	return SetReturn(args, &scope, GetNull(isolate));
}
static V8Return _physicsSetAngularVelocity(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPphysicsDynamic* actor = (OPphysicsDynamic*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_DYNAMIC);

	f32 x = args[1 - inScope]->NumberValue();
	f32 y = args[2 - inScope]->NumberValue();
	f32 z = args[3 - inScope]->NumberValue();

	OPphysicsSetAngularVelocity(actor, x, y, z);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsGetTransform(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPphysicsActor* actor = (OPphysicsActor*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_DYNAMIC);
	if (actor == NULL) {
		actor = (OPphysicsActor*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_STATIC);
		if (actor == NULL) {
			OPlog("No Actor Given");
			return SetReturn(args, &scope, GetNull(isolate));
		}
	}

	OPmat4* mat = (OPmat4*)GetArgPointer(args, isolate, 1 - inScope);

	OPphysicsGetTransform(actor, mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _physicsCreateBoxDynamic(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);

	f32 x = 0, y = 0, z = 0, sx = 1, sy = 1, sz = 1;
	
	if (args.Length() > (1 - inScope)) {
		V8Object obj = args[1 - inScope]->ToObject();
		x = GetNumber(isolate, obj, "x", 0);
		y = GetNumber(isolate, obj, "y", 0);
		z = GetNumber(isolate, obj, "z", 0);
		sx = GetNumber(isolate, obj, "sx", 0);
		sy = GetNumber(isolate, obj, "sy", 0);
		sz = GetNumber(isolate, obj, "sz", 0);
	}

	OPphysicsDynamic* actor = OPphysicsCreateBoxDynamic(scene, x, y, z, sx, sy, sz);

	V8Object obj = CreateTypedObject(isolate, actor, OPscript_PHYSICS_DYNAMIC);
	SetFunction(isolate, obj, "AddForce", _physicsAddForce);
	SetFunction(isolate, obj, "AddTorque", _physicsAddTorque);
	SetFunction(isolate, obj, "SetLinearVelocity", _physicsSetLinearVelocity);
	SetFunction(isolate, obj, "SetAngularVelocity", _physicsSetAngularVelocity);
	SetFunction(isolate, obj, "GetTransform", _physicsGetTransform);

	return SetReturn(args, &scope, obj);
}

static V8Return _physicsCreateSphereDynamic(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);

	f32 x = 0, y = 0, z = 0, s = 1;

	if (args.Length() > (1 - inScope)) {
		V8Object obj = args[1 - inScope]->ToObject();
		x = GetNumber(isolate, obj, "x", 0);
		y = GetNumber(isolate, obj, "y", 0);
		z = GetNumber(isolate, obj, "z", 0);
		s = GetNumber(isolate, obj, "s", 0);
	}

	OPphysicsDynamic* actor = OPphysicsCreateSphereDynamic(scene, x, y, z, s);

	V8Object obj = CreateTypedObject(isolate, actor, OPscript_PHYSICS_DYNAMIC);
	SetFunction(isolate, obj, "AddForce", _physicsAddForce);
	SetFunction(isolate, obj, "AddTorque", _physicsAddTorque);
	SetFunction(isolate, obj, "SetLinearVelocity", _physicsSetLinearVelocity);
	SetFunction(isolate, obj, "SetAngularVelocity", _physicsSetAngularVelocity);
	SetFunction(isolate, obj, "GetTransform", _physicsGetTransform);

	return SetReturn(args, &scope, obj);
}

static V8Return _physicsCreateBoxStatic(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);

	f32 x = 0, y = 0, z = 0, sx = 1, sy = 1, sz = 1;

	if (args.Length() > (1 - inScope)) {
		V8Object obj = args[1 - inScope]->ToObject();
		x = GetNumber(isolate, obj, "x", 0);
		y = GetNumber(isolate, obj, "y", 0);
		z = GetNumber(isolate, obj, "z", 0);
		sx = GetNumber(isolate, obj, "sx", 0);
		sy = GetNumber(isolate, obj, "sy", 0);
		sz = GetNumber(isolate, obj, "sz", 0);
	}

	OPphysicsStatic* actor = OPphysicsCreateBoxStatic(scene, x, y, z, sx, sy, sz);

	V8Object obj = CreateTypedObject(isolate, actor, OPscript_PHYSICS_STATIC);
	SetFunction(isolate, obj, "GetTransform", _physicsGetTransform);

	return SetReturn(args, &scope, obj);
}

static V8Return _physicsCreateSphereStatic(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);

	f32 x = 0, y = 0, z = 0, s = 1;

	if (args.Length() > (1 - inScope)) {
		V8Object obj = args[1 - inScope]->ToObject();
		x = GetNumber(isolate, obj, "x", 0);
		y = GetNumber(isolate, obj, "y", 0);
		z = GetNumber(isolate, obj, "z", 0);
		s = GetNumber(isolate, obj, "s", 0);
	}

	OPphysicsStatic* actor = OPphysicsCreateSphereStatic(scene, x, y, z, s);

	V8Object obj = CreateTypedObject(isolate, actor, OPscript_PHYSICS_STATIC);
	SetFunction(isolate, obj, "GetTransform", _physicsGetTransform);

	return SetReturn(args, &scope, obj);
}

static V8Return _physicsCreatePlane(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPphysicsScene* scene = (OPphysicsScene*)GetFirstPointer(args, isolate, &inScope, OPscript_PHYSICS_SCENE);
	
	void* actor = OPphysicsCreatePlane(scene);

	V8Object obj = CreateTypedObject(isolate, actor, OPscript_PHYSICS_STATIC);

	return SetReturn(args, &scope, obj);
}


#endif