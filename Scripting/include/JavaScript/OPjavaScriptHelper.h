#ifndef OPIFEXENGINE_OPSCRIPTNODEHELPER
#define OPIFEXENGINE_OPSCRIPTNODEHELPER

#include "./Core/include/OPtypes.h"



#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#if defined(OPIFEX_OPTION_NODEJS)
#include <node.h>
#include <node_object_wrap.h>
#endif

#include "v8.h"
using namespace v8;



///////////////////////////////
// SPECIFIC TO NODEJS 0.10.x
///////////////////////////////
#ifdef OPIFEX_NODEJS_0_10

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope;

typedef Arguments JS_ARGS;
typedef Handle<Value> JS_RETURN_VAL;

#define JS_NEW_OBJECT() Object::New()
#define JS_NEW_OBJECT_TEMPLATE() ObjectTemplate::New()
#define JS_NEW_STRING(name) String::New(name)
#define JS_NEW_NUMBER(val) Number::New(val)
#define JS_NEW_BOOL(val) Boolean::New(val)
#define JS_NEW_ARRAY() Array::New()
#define JS_NEW_FUNCTION(func) Local<Function>::New(func)
#define JS_NEW_FUNCTION_TEMPLATE(func) FunctionTemplate::New(func)
#define JS_NEW_CONTEXT(obj) Context::New(NULL, obj)
#define JS_NEW_NULL() Null()
#define JS_RETURN(val) return val;
#define JS_RETURN_NULL JS_RETURN(JS_NEW_NULL())

#define JS_SET_METHOD(obj, name, func) obj->Set(String::New(name), FunctionTemplate::New(func)->GetFunction());
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);


#endif




///////////////////////////////
// SPECIFIC TO NODEJS 0.12.x
///////////////////////////////
#ifdef OPIFEX_NODEJS_0_12

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope(isolate);


typedef FunctionCallbackInfo<Value> JS_ARGS;
typedef void JS_RETURN_VAL;

#define JS_NEW_OBJECT() Object::New(isolate)
#define JS_NEW_OBJECT_TEMPLATE() ObjectTemplate::New(isolate)
#define JS_NEW_STRING(name) String::NewFromUtf8(isolate, name)
#define JS_NEW_NUMBER(val) Number::New(isolate, val)
#define JS_NEW_BOOL(val) Boolean::New(isolate, val)
#define JS_NEW_ARRAY() Array::New(isolate)
#define JS_NEW_FUNCTION(func) Local<Function>::New(isolate, func)
#define JS_NEW_FUNCTION_TEMPLATE(func) FunctionTemplate::New(isolate, func)
#define JS_NEW_CONTEXT(obj) Context::New(isolate, NULL, obj)
#define JS_NEW_NULL() Null(isolate)
#define JS_RETURN(val) args.GetReturnValue().Set(val);
#define JS_RETURN_NULL ;

#define JS_SET_METHOD(obj, name, func) NODE_SET_METHOD(obj, name, func)
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);


#endif



///////////////////////////////
// SPECIFIC TO V8
///////////////////////////////
#ifdef OPIFEX_OPTION_V8

extern Isolate* isolate;

#define SCOPE_AND_ISOLATE   Isolate::Scope isolate_scope(isolate); \
                            HandleScope scope(isolate);

typedef FunctionCallbackInfo<Value> JS_ARGS;
typedef void JS_RETURN_VAL;

#define JS_NEW_OBJECT() Object::New(isolate)
#define JS_NEW_OBJECT_TEMPLATE() ObjectTemplate::New(isolate)
#define JS_NEW_STRING(name) String::NewFromUtf8(isolate, name)
#define JS_NEW_NUMBER(val) Number::New(isolate, val)
#define JS_NEW_BOOL(val) Boolean::New(isolate, val)
#define JS_NEW_ARRAY() Array::New(isolate)
#define JS_NEW_FUNCTION(func) Local<Function>::New(isolate, func)
#define JS_NEW_FUNCTION_TEMPLATE(func) FunctionTemplate::New(isolate, func)
#define JS_NEW_CONTEXT(obj) Context::New(isolate, NULL, obj)
#define JS_NEW_NULL() Null(isolate)
#define JS_RETURN(val) args.GetReturnValue().Set(val);
#define JS_RETURN_NULL ;

#define JS_SET_METHOD(obj, name, func) obj->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);
#define JS_SET_PROTOTYPE_METHOD(obj, name, func) obj->PrototypeTemplate()->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#endif



#define JS_SET_NUMBER(obj, name, val) obj->Set(JS_NEW_STRING(name), JS_NEW_NUMBER(val));
#define JS_SET_STRING(obj, name, val) obj->Set(JS_NEW_STRING(name), JS_NEW_STRING(val));
#define JS_GET_NUMBER(obj, name) obj->Get(JS_NEW_STRING(name))->NumberValue();
#define JS_SET_PTR(obj, ptr) JS_SET_NUMBER(obj, "ptr", (OPint)ptr)
#define JS_GET_PTR(obj, type) (type*)(OPint)obj->Get(JS_NEW_STRING("ptr"))->IntegerValue();
#define JS_GET_ARG_PTR(args, ind, type) JS_GET_PTR(args[ind]->ToObject(), type)

#define JS_SETUP_INSTANCE const unsigned argc = 1; \
    Handle<Value> argv[argc] = { args[0] }; \
    Local<Function> cons = JS_NEW_FUNCTION(constructor); \
    Local<Object> instance = cons->NewInstance(argc, argv);


typedef Persistent<Script, CopyablePersistentTraits<Script> > OPjavaScriptPersistentScript;
typedef Persistent<Context, CopyablePersistentTraits<Context> > OPjavaScriptPersistentContext;



#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
