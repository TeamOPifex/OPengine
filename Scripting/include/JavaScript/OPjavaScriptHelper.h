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


#if defined(_DEBUG)
#else
#define NDEBUG
#endif


extern OPuint _JS_ARGC;
#define JS_BEGIN_ARGS _JS_ARGC = 0;
#define JS_BEGIN_ARGS_SELF _JS_ARGC = -1;


///////////////////////////////
// SPECIFIC TO NODEJS 0.10.x
///////////////////////////////
#ifdef OPIFEX_NODEJS_0_10

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope;

typedef Arguments JS_ARGS;
typedef Handle<Value> JS_RETURN_VAL;

typedef Persistent<Script> OPjavaScriptPersistentScript;
typedef Persistent<Context> OPjavaScriptPersistentContext;
typedef Persistent<Value> OPjavaScriptPersistentValue;
typedef Persistent<Function> OPjavaScriptPersistentFunction;
typedef Persistent<Object> OPjavaScriptPersistentObject;

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
#define JS_NEW_PERSISTENT_FUNCTION(method) OPjavaScriptPersistentFunction(method);
#define JS_RETURN(val) return val;
#define JS_RETURN_NULL JS_RETURN(JS_NEW_NULL())

#define JS_SET_METHOD(obj, name, func) obj->Set(String::New(name), FunctionTemplate::New(func)->GetFunction());
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);
#define JS_SET_PROTOTYPE_METHOD(obj, name, func) obj->PrototypeTemplate()->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#define JS_SET_NUMBER(target, name, number) target->Set(JS_NEW_STRING(name), JS_NEW_NUMBER(number));



#define JS_RUN_SELF(m) \
JS_BEGIN_ARGS_SELF \
JS_RETURN(m(args));

#define JS_RUN(m) \
JS_BEGIN_ARGS \
JS_RETURN(m(args));

#endif




///////////////////////////////
// SPECIFIC TO NODEJS 0.12.x
///////////////////////////////
#ifdef OPIFEX_NODEJS_0_12

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope(isolate);


typedef FunctionCallbackInfo<Value> JS_ARGS;
typedef void JS_RETURN_VAL;
typedef Persistent<Script, CopyablePersistentTraits<Script> > OPjavaScriptPersistentScript;
typedef Persistent<Context, CopyablePersistentTraits<Context> > OPjavaScriptPersistentContext;
typedef Persistent<Value, CopyablePersistentTraits<Value> > OPjavaScriptPersistentValue;
typedef Persistent<Function, CopyablePersistentTraits<Function> > OPjavaScriptPersistentFunction;
typedef Persistent<Object, CopyablePersistentTraits<Object> > OPjavaScriptPersistentObject;

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
#define JS_NEW_PERSISTENT_FUNCTION(method) OPjavaScriptPersistentFunction(isolate, method);
#define JS_RETURN(val) args.GetReturnValue().Set(val);
#define JS_RETURN_NULL ;

#define JS_SET_METHOD(obj, name, func) NODE_SET_METHOD(obj, name, func)
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);
#define JS_SET_PROTOTYPE_METHOD(obj, name, func) obj->PrototypeTemplate()->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#define JS_SET_NUMBER(target, name, number) target->Set(JS_NEW_STRING(name), JS_NEW_NUMBER(number));




#define JS_RUN_SELF(m) \
JS_BEGIN_ARGS_SELF \
m(args);

#define JS_RUN(m) \
JS_BEGIN_ARGS \
m(args);

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

typedef Persistent<Script, CopyablePersistentTraits<Script> > OPjavaScriptPersistentScript;
typedef Persistent<Context, CopyablePersistentTraits<Context> > OPjavaScriptPersistentContext;
typedef Persistent<Value, CopyablePersistentTraits<Value> > OPjavaScriptPersistentValue;
typedef Persistent<Object, CopyablePersistentTraits<Object> > OPjavaScriptPersistentObject;
typedef Persistent<Function, CopyablePersistentTraits<Function> > OPjavaScriptPersistentFunction;

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
#define JS_NEW_PERSISTENT_FUNCTION(method) OPjavaScriptPersistentFunction(isolate, method);
#define JS_RETURN(val) args.GetReturnValue().Set(val);
#define JS_RETURN_NULL ;

#define JS_SET_METHOD(obj, name, func) obj->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#define JS_SET_OBJECT(target, name, object) target->Set(JS_NEW_STRING(name), object);
#define JS_SET_PROTOTYPE_METHOD(obj, name, func) obj->PrototypeTemplate()->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func)->GetFunction());
#define JS_SET_NUMBER(target, name, number) target->Set(JS_NEW_STRING(name), JS_NEW_NUMBER(number));




#define JS_RUN_SELF(m) \
JS_BEGIN_ARGS_SELF \
m(args);

#define JS_RUN(m) \
JS_BEGIN_ARGS \
m(args);


#endif



///////////////////////////////
// COMMON TO EACH NODE_10, NODE_12, V8
///////////////////////////////

//#define JS_SET_OBJECT(obj, name, val) obj->Set(JS_NEW_STRING(name), val);
//#define JS_SET_NUMBER(obj, name, val) obj->Set(JS_NEW_STRING(name), JS_NEW_NUMBER(val));
#define JS_SET_STRING(obj, name, val) obj->Set(JS_NEW_STRING(name), JS_NEW_STRING(val));
#define JS_GET_NUMBER(obj, name) obj->Get(JS_NEW_STRING(name))->NumberValue();
#define JS_GET_INTEGER(obj, name) obj->Get(JS_NEW_STRING(name))->IntegerValue();
#define JS_SET_PTR(obj, ptr) JS_SET_NUMBER(obj, "ptr", (OPint)ptr)
#define JS_GET_PTR(obj, type) (type*)(OPint)obj->Get(JS_NEW_STRING("ptr"))->IntegerValue();
#define JS_GET_ARG_PTR(args, ind, type) JS_GET_PTR(args[ind]->ToObject(), type)

#define JS_SETUP_INSTANCE const unsigned argc = 1; \
    Handle<Value> argv[argc] = { args[0] }; \
    Local<Function> cons = JS_NEW_FUNCTION(constructor); \
    Local<Object> instance = cons->NewInstance(argc, argv);

#define JS_GET_PERSISTENT_NUMBER(obj) Local<Value>::New(isolate, obj)->NumberValue()
#define JS_CREATE_PERSISTENT(value) Persistent<Value>(isolate, value)




inline void JS_SET_OBJECT_PTR(Handle<Object> obj, const OPchar* name, void* ptr) {
	SCOPE_AND_ISOLATE

    Handle<Object> tmp = JS_NEW_OBJECT();
    JS_SET_PTR(tmp, ptr);
    JS_SET_OBJECT(obj, name, tmp);
}


inline void* _JS_NEXT(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE
    _JS_ARGC++;
    if(_JS_ARGC == 0) {
        return JS_GET_PTR(args.This(), void);
    }
    return JS_GET_ARG_PTR(args, _JS_ARGC - 1, void);
}
#define JS_NEXT_ARG_AS(t) (t*)_JS_NEXT(args);




#define JS_MAKE_WRAPPED_FN_NAME(x) _ ## x
#define JS_MAKE_SELF_FN_NAME(x) x ## Self

#define JS_HELPER_SELF_WRAPPER(funcName) \
JS_RETURN_VAL JS_MAKE_WRAPPED_FN_NAME(funcName)(const JS_ARGS& args); \
JS_RETURN_VAL funcName(const JS_ARGS& args) { JS_RUN(JS_MAKE_WRAPPED_FN_NAME(funcName)) } \
JS_RETURN_VAL JS_MAKE_SELF_FN_NAME(funcName)(const JS_ARGS& args) { JS_RUN_SELF(JS_MAKE_WRAPPED_FN_NAME(funcName)) } \
JS_RETURN_VAL JS_MAKE_WRAPPED_FN_NAME(funcName)(const JS_ARGS& args)

#endif


#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
