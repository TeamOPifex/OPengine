#include "./Scripting/include/JavaScript/Performance/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Performance/include/OPphysX.h"

JS_RETURN_VAL _OPphysXInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
	OPphysXInit();
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXCreateTriangleMesh(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    ui32 vertCount = args[0]->IntegerValue();
    PxVec3* verts = JS_GET_ARG_PTR(args, 1, PxVec3);
    ui32 triCount = args[2]->IntegerValue();
    PxU32* indices = JS_GET_ARG_PTR(args, 3, PxU32);

	PxTriangleMesh* ptr = OPphysXCreateTriangleMesh(vertCount, verts, triCount, indices);

	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXSetFilter(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    ui32 filterGroup = args[1]->IntegerValue();
    ui32 filterMask = args[2]->IntegerValue();
	OPphysXSetFilter(actor, filterGroup, filterMask);
	
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXCreateMaterial(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    f32 staticFriction = args[0]->NumberValue();
    f32 dynamicFriction = args[1]->NumberValue();
    f32 restitution = args[2]->NumberValue();
	OPphysXMaterial* ptr = OPphysXCreateMaterial(staticFriction, dynamicFriction, restitution);
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXAddSphereShape(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
    OPfloat size = args[2]->NumberValue();
	OPphysXShape* ptr = OPphysXAddSphereShape(actor, material, size);
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXAddBoxShape(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
    OPvec3* size = JS_GET_ARG_PTR(args, 2, OPvec3);
	OPphysXShape* ptr = OPphysXAddBoxShape(actor, material, *size);
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXAddPlaneShape(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
	OPphysXShape* ptr = OPphysXAddPlaneShape(actor, material);
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}

JS_RETURN_VAL _OPphysXAddTriangleMeshShape(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXMaterial* material = JS_GET_ARG_PTR(args, 1, OPphysXMaterial);
    PxTriangleMesh* mesh = JS_GET_ARG_PTR(args, 2, PxTriangleMesh);
	OPphysXShape* ptr = OPphysXAddTriangleMeshShape(actor, material, mesh);
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}


JS_RETURN_VAL _OPphysXGetTransform(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPmat4* mat = JS_GET_ARG_PTR(args, 1, OPmat4);
    OPphysXGetTransform(actor, mat);

	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXShutdown(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
	OPphysXShutdown();
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetMass(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidDynamic* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidDynamic);
    OPfloat amount = args[1]->NumberValue();
    OPphysXSetMass(actor, amount);
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXAddForce(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidDynamic* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidDynamic);
    OPvec3* force = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXAddForce(actor, *force);
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXAddTorque(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidDynamic* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidDynamic);
    OPvec3* torque = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXAddTorque(actor, *torque);
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetLinearVelocity(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidDynamic* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidDynamic);
    OPvec3* velocity = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXSetLinearVelocity(actor, *velocity);
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetAngularVelocity(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidDynamic* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidDynamic);
    OPvec3* velocity = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPphysXSetAngularVelocity(actor, *velocity);
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetGravity(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXSetGravity(actor, args[1]->IntegerValue());
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetSimulation(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXShape* shape = JS_GET_ARG_PTR(args, 0, OPphysXShape);
    OPphysXSetSimulation(shape, args[1]->IntegerValue());
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetTrigger(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXShape* shape = JS_GET_ARG_PTR(args, 0, OPphysXShape);
    OPphysXSetTrigger(shape, args[1]->IntegerValue());
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXSetSceneQuery(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXShape* shape = JS_GET_ARG_PTR(args, 0, OPphysXShape);
    OPphysXSetSceneQuery(shape, args[1]->IntegerValue());
    
	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPphysXOverlapping(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
    OPphysXRigidActor* other = JS_GET_ARG_PTR(args, 1, OPphysXRigidActor);

    OPint result = OPphysXOverlapping(actor, other);
    
	JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPphysXGetShape(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPphysXRigidActor* actor = JS_GET_ARG_PTR(args, 0, OPphysXRigidActor);
	OPphysXShape* ptr = OPphysXGetShape(actor, args[1]->IntegerValue());
	
	Handle<Object> result = JS_NEW_OBJECT();
	JS_SET_PTR(result, ptr);

	JS_RETURN(result);
}


void OPphysXWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> physX = JS_NEW_OBJECT();
	JS_SET_METHOD(physX, "Init", _OPphysXInit);
	JS_SET_METHOD(physX, "CreateTriangleMesh", _OPphysXCreateTriangleMesh);
	JS_SET_METHOD(physX, "SetFilter", _OPphysXSetFilter);
	JS_SET_METHOD(physX, "CreateMaterial", _OPphysXCreateMaterial);
	JS_SET_METHOD(physX, "AddSphereShape", _OPphysXAddSphereShape);
	JS_SET_METHOD(physX, "AddBoxShape", _OPphysXAddBoxShape);
	JS_SET_METHOD(physX, "AddPlaneShape", _OPphysXAddPlaneShape);
	JS_SET_METHOD(physX, "AddTriangleMeshShape", _OPphysXAddTriangleMeshShape);
	JS_SET_METHOD(physX, "GetTransform", _OPphysXGetTransform);
	JS_SET_METHOD(physX, "Overlapping", _OPphysXOverlapping);
	JS_SET_METHOD(physX, "Shutdown", _OPphysXShutdown);

	JS_SET_METHOD(physX, "SetMass", _OPphysXSetMass);
	JS_SET_METHOD(physX, "AddForce", _OPphysXAddForce);
	JS_SET_METHOD(physX, "AddTorque", _OPphysXAddTorque);
	JS_SET_METHOD(physX, "SetLinearVelocity", _OPphysXSetLinearVelocity);
	JS_SET_METHOD(physX, "SetAngularVelocity", _OPphysXSetAngularVelocity);
	JS_SET_METHOD(physX, "SetGravity", _OPphysXSetGravity);
	JS_SET_METHOD(physX, "SetSimulation", _OPphysXSetSimulation);
	JS_SET_METHOD(physX, "SetSceneQuery", _OPphysXSetSceneQuery);
	JS_SET_METHOD(physX, "SetTrigger", _OPphysXSetTrigger);
	JS_SET_METHOD(physX, "GetShape", _OPphysXGetShape);
	JS_SET_OBJECT(exports, "physX", physX);
    
}

#endif