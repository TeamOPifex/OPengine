// #include "./Scripting/include/JavaScript/Math/Wrappers.h"

// #if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

// #include "./Math/include/OPvec4.h"

// JS_RETURN_VAL _OPvec4LogSelf(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;

//     OPvec4* ptr = JS_GET_PTR(args.This(), OPvec4);
//     String::Utf8Value str(args[0]->ToString());
//     OPvec4Log(*str, *ptr);

//     JS_RETURN_NULL;
// }

// JS_RETURN_VAL _OPvec4Log(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;

//     OPvec4* ptr = JS_GET_ARG_PTR(args, 1, OPvec4);
//     String::Utf8Value str(args[0]->ToString());
//     OPvec4Log(*str, *ptr);

//     JS_RETURN_NULL;
// }

// JS_RETURN_VAL _OPvec4SetSelf(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;

//     OPlog("Setting Vec4 Self");

//     OPvec4* ptr = JS_GET_PTR(args.This(), OPvec4);
//     ptr->x = args[0]->NumberValue();
//     ptr->y = args[1]->NumberValue();
//     ptr->z = args[2]->NumberValue();
//     ptr->w = args[3]->NumberValue();

//     JS_RETURN_NULL;
// }

// JS_RETURN_VAL _OPvec4Set(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;

//     OPlog("Setting Vec4");

//     OPvec4* ptr = JS_GET_ARG_PTR(args, 0, OPvec4);
//     ptr->x = args[1]->NumberValue();
//     ptr->y = args[2]->NumberValue();
//     ptr->z = args[3]->NumberValue();
//     ptr->w = args[4]->NumberValue();

//     JS_RETURN_NULL;
// }

// Handle<Object> _OPvec4Setup(Handle<Object> result, OPvec4* ptr){
//     SCOPE_AND_ISOLATE;

//     JS_SET_PTR(result, ptr);
//     JS_SET_METHOD(result, "Log", _OPvec4LogSelf);
//     JS_SET_METHOD(result, "Set", _OPvec4SetSelf);

//     return result;
// }

// JS_RETURN_VAL _OPvec4Create(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;

//     OPvec4* ptr = (OPvec4*)OPalloc(sizeof(OPvec4));
//     if(args.Length() == 4) {
//         *ptr = OPvec4Create(args[0]->NumberValue(),args[1]->NumberValue(),args[2]->NumberValue(),args[3]->NumberValue());
//     } else {
//         *ptr = OPvec4Create(0,0,0,0);
//     }
    
//     Handle<Object> result = JS_NEW_OBJECT();
//     JS_RETURN(_OPvec4Setup(result, ptr));
// }

// void OPphysicsWrapper(Handle<Object> exports) {
//     SCOPE_AND_ISOLATE;

//     Handle<Object> physics = JS_NEW_OBJECT();
//     JS_SET_METHOD(physics, "Init", _OPphysicsInit);
//     JS_SET_METHOD(physics, "Shutdown", _OPphysicsShutdown);
//     JS_SET_OBJECT(exports, "physics", physics);

//     Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPphysicsSceneCreate);
//     Handle<Object> physicsScene = tpl->GetFunction();
//     JS_SET_METHOD(physicsScene, "Create", _OPphysicsSceneCreate);
//     JS_SET_METHOD(physicsScene, "Step", _OPphysicsSceneStep);
//     JS_SET_METHOD(physicsScene, "CreateBoxDynamic", _OPphysicsSceneCreateBoxDynamic);
//     JS_SET_METHOD(physicsScene, "CreateSphereDynamic", _OPphysicsSceneCreateSphereDynamic);
//     JS_SET_METHOD(physicsScene, "CreateBoxStatic", _OPphysicsSceneCreateBoxStatic);
//     JS_SET_METHOD(physicsScene, "CreateSphereStatic", _OPphysicsSceneCreateSphereStatic);
//     JS_SET_METHOD(physicsScene, "CreatePlane", _OPphysicsSceneCreatePlane);
//     JS_SET_METHOD(physicsScene, "Destroy", _OPphysicsSceneDestroy);
//     JS_SET_NUMBER(physicsScene, "size", sizeof(OPphysicsScene));
//     JS_SET_OBJECT(exports, "physicsScene", physicsScene);
    
//     Local<FunctionTemplate> tpl2 = JS_NEW_FUNCTION_TEMPLATE(_OPphysicsDynamicCreate);
//     Handle<Object> physicsDynamic = tpl2->GetFunction();
//     JS_SET_METHOD(physicsDynamic, "Create", _OPphysicsDynamicCreate);
//     JS_SET_METHOD(physicsDynamic, "AddForce", _OPphysicsSceneStep);
//     JS_SET_METHOD(physicsDynamic, "AddTorque", _OPphysicsSceneCreateBoxDynamic);
//     JS_SET_METHOD(physicsDynamic, "SetLinearVelocity", _OPphysicsSceneCreateSphereDynamic);
//     JS_SET_METHOD(physicsDynamic, "SetAngularVelocity", _OPphysicsSceneCreateBoxStatic);
//     JS_SET_NUMBER(physicsDynamic, "size", sizeof(OPphysicsDynamic));
//     JS_SET_OBJECT(exports, "physicsDynamic", physicsDynamic);
    
// }

// #endif