#pragma once

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
#ifndef NDEBUG
#define NDEBUG
#endif
#endif


extern i32 _JS_ARGC;
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
#define JS_NEW_INTEGER(val) Integer::New(isolate, val)
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
#define JS_SET_PROTOTYPE_METHOD(obj, name, func) obj->PrototypeTemplate()->Set(JS_NEW_STRING(name), JS_NEW_FUNCTION_TEMPLATE(func));
#define JS_SET_NUMBER(target, name, number) target->Set(JS_NEW_STRING(name), JS_NEW_NUMBER((double)number));
#define JS_SET_INTEGER(target, name, number) target->Set(JS_NEW_STRING(name), JS_NEW_INTEGER(number));


#define JS_VALUE_TO_ARRAY(A) Handle<Array>::Cast(A)
#define JS_VALUE_TO_OBJECT(A) A->ToObject()
#define JS_OBJECT_GET(A, B) A->Get(JS_NEW_STRING(B))
#define JS_OBJECT_GET_INT(A, B) JS_OBJECT_GET(A,B)->IntegerValue();
// Warning the * will be invalid if used outside of the scope
#define JS_OBJECT_GET_STRING(A, B) *String::Utf8Value(JS_OBJECT_GET(A,B)->ToString())
#define JS_TO_STRING(A) *String::Utf8Value(A->ToString())

#define JS_RUN_SELF(m) \
JS_BEGIN_ARGS_SELF \
m(args);

#define JS_RUN(m) \
JS_BEGIN_ARGS \
m(args);

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);

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
#define JS_GET_ARG_PTR(args, ind, type) JS_GET_PTR(args[(int)ind]->ToObject(), type)
#define JS_GET_ARG_OPUINT(args, ind) ((OPint)args[(int)ind]->IntegerValue())

#ifdef _DEBUG
    #define JS_SET_TYPE(obj, t) JS_SET_STRING(obj, "type", t)
    inline const OPchar* JS_GET_TYPE(Handle<Object> obj) {
        String::Utf8Value str(obj->Get(JS_NEW_STRING("type"))->ToString());
        return *str;
    }
    #define JS_GET_ARG_TYPE(args, ind) JS_GET_TYPE(args[ind]->ToObject())
    #define JS_ASSERT_ARG_PTR_TYPE(args, ind, type, message) ASSERT(OPstringEquals(JS_GET_ARG_TYPE(args, ind), type), message)
#else
    #define JS_SET_TYPE(obj, t)
    #define JS_GET_ARG_TYPE(args, ind)
    #define JS_ASSERT_ARG_PTR_TYPE(args, ind, type, message)
    inline const OPchar* JS_GET_TYPE(Handle<Object> obj) {
        return NULL;
    }
#endif

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


inline OPuint _JS_NEXT_OPUINT(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE
    _JS_ARGC++;
    if(_JS_ARGC == 0) {
        return (OPuint)args.This()->IntegerValue();
    }
    return JS_GET_ARG_OPUINT(args, _JS_ARGC - 1);
}
#define JS_NEXT_ARG_AS_OPUINT() _JS_NEXT_OPUINT(args);


inline Handle<Value> _JS_NEXT_ARG(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE
    _JS_ARGC++;
    if(_JS_ARGC == 0) {
        return args.This();
    }
    return args[(int)(_JS_ARGC - 1)];
}
#define JS_NEXT_ARG_VAL() _JS_NEXT_ARG(args);



#define JS_MAKE_WRAPPED_FN_NAME(x) _ ## x
#define JS_MAKE_SELF_FN_NAME(x) x ## Self

// Makes 2 methods out of 1 that will make it a callable from the object itself
// ex: VectorAdd(vector, [1, 1, 1]) vs vector.Add([1,1,1])
#define JS_HELPER_SELF_WRAPPER(funcName) \
JS_RETURN_VAL JS_MAKE_WRAPPED_FN_NAME(funcName)(const JS_ARGS& args); \
JS_RETURN_VAL funcName(const JS_ARGS& args) { JS_RUN(JS_MAKE_WRAPPED_FN_NAME(funcName)) } \
JS_RETURN_VAL JS_MAKE_SELF_FN_NAME(funcName)(const JS_ARGS& args) { JS_RUN_SELF(JS_MAKE_WRAPPED_FN_NAME(funcName)) } \
JS_RETURN_VAL JS_MAKE_WRAPPED_FN_NAME(funcName)(const JS_ARGS& args)




#define JS_HELPER_EASY_DESTROY(t) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## Destroy) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    t ## Destroy(ptr);\
    JS_RETURN_NULL; \
}


// EASY Wrappers for when it's just ptr passing
#define JS_HELPER_EASY_FREE(t) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## Free) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    t ## Free(ptr);\
    JS_RETURN_NULL; \
}

#define JS_HELPER_EASY_SELF_WRAPPER_1(t, m, arg1) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## m) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    arg1* a1 = JS_NEXT_ARG_AS(arg1); \
    t ## m(ptr, a1);\
    JS_RETURN_NULL; \
}

#define JS_HELPER_EASY_SELF_WRAPPER_2(t, m, arg1) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## m) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    arg1* a1 = JS_NEXT_ARG_AS(arg1); \
    arg2* a2 = JS_NEXT_ARG_AS(arg2); \
    t ## m(ptr, a1, a2);\
    JS_RETURN_NULL; \
}

#define JS_HELPER_EASY_SELF_WRAPPER_1_RETURN(t, m, arg1, r) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## m) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    arg1* a1 = JS_NEXT_ARG_AS(arg1); \
    void* result = t ## m(ptr, a1);\
    JS_RETURN(r(JS_NEW_OBJECT(), result)); \
}

#define JS_HELPER_EASY_SELF_WRAPPER_2_RETURN(t, m, arg1, arg2, r) \
JS_HELPER_SELF_WRAPPER( JS_MAKE_WRAPPED_FN_NAME(t ## m) ) { \
    SCOPE_AND_ISOLATE \
    t* ptr = JS_NEXT_ARG_AS(t); \
    arg1* a1 = JS_NEXT_ARG_AS(arg1); \
    arg2* a2 = JS_NEXT_ARG_AS(arg2); \
    void* result = t ## m(ptr, a1, a2);\
    JS_RETURN(r(JS_NEW_OBJECT(), result)); \
}

// Helps with wrapping
#define JS_WRAP_(WRAPPER,EXTERNAL,NAME,PTR) JS_SET_OBJECT(EXTERNAL, NAME, WRAPPER(JS_NEW_OBJECT(), PTR));
#define JS_WRAP(EXTERNAL,NAME,PTR) JS_WRAP_(OPjavaScriptWrap, EXTERNAL,NAME,PTR)
#define JS_EASY_WRAP_(TYPE) inline Handle<Object> OPjavaScriptWrap(Handle<Object> result, TYPE* ptr) { return TYPE ## Wrapper(result, ptr); }
#define JS_EASY_WRAP_PTR(TYPE) Handle<Object> \
    TYPE ## Wrapper(Handle<Object> result, TYPE* ptr); \
    JS_EASY_WRAP_(TYPE)



#define JS_NEXT_ARG_AS_NUMBER args[_JS_ARGC]->NumberValue(); _JS_ARGC++;
#define JS_NEXT_ARG_AS_INTEGER args[_JS_ARGC]->IntegerValue(); _JS_ARGC++;
#define JS_NEXT_ARG_AS_STRING(name) String::Utf8Value name ## string(args[_JS_ARGC]->ToString()); _JS_ARGC++; const OPchar* name = *(name ## string);
#define JS_NEXT_ARG_AS_VEC3 OPvec3((OPfloat)args[_JS_ARGC]->NumberValue(), (OPfloat)args[_JS_ARGC + 1]->NumberValue(), (OPfloat)args[_JS_ARGC + 2]->NumberValue()); _JS_ARGC+= 3;

#endif
