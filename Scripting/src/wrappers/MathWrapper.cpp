#include "./Scripting/include/wrappers/MathWrapper.h"

#ifdef OPIFEX_V8

#include <v8.h>

static void _OPmat4Create(const FunctionCallbackInfo<Value>& args);
static void _OPmat4Destroy(const FunctionCallbackInfo<Value>& args);
static void _OPmat4RotateX(const FunctionCallbackInfo<Value>& args);
static void _OPmat4RotateY(const FunctionCallbackInfo<Value>& args);
static void _OPmat4RotateZ(const FunctionCallbackInfo<Value>& args);
static void _OPmat4SetRotateX(const FunctionCallbackInfo<Value>& args);
static void _OPmat4SetRotateY(const FunctionCallbackInfo<Value>& args);
static void _OPmat4SetRotateZ(const FunctionCallbackInfo<Value>& args);
static void _OPmat4SetVec3(const FunctionCallbackInfo<Value>& args);
static void _OPmat4Identity(const FunctionCallbackInfo<Value>& args);

static void _OPvec3Create(const FunctionCallbackInfo<Value>& args);
static void _OPvec3CreateFromPointer(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Destroy(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Set(const FunctionCallbackInfo<Value>& args);
static void _OPvec3X(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Y(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Z(const FunctionCallbackInfo<Value>& args);
static void _OPvec3XYZ(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Add(const FunctionCallbackInfo<Value>& args);
static void _OPvec3Sub(const FunctionCallbackInfo<Value>& args);

void MathInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target) {

	// OP.mat4
	Local<ObjectTemplate> mat4 = ObjectTemplate::New(isolate);
	mat4->Set(String::NewFromUtf8(isolate, "Create"), FunctionTemplate::New(isolate, _OPmat4Create));
	mat4->Set(String::NewFromUtf8(isolate, "Destroy"), FunctionTemplate::New(isolate, _OPmat4Destroy));
	mat4->Set(String::NewFromUtf8(isolate, "RotX"), FunctionTemplate::New(isolate, _OPmat4RotateX));
	mat4->Set(String::NewFromUtf8(isolate, "RotY"), FunctionTemplate::New(isolate, _OPmat4RotateY));
	mat4->Set(String::NewFromUtf8(isolate, "RotZ"), FunctionTemplate::New(isolate, _OPmat4RotateZ));
	mat4->Set(String::NewFromUtf8(isolate, "SetRotX"), FunctionTemplate::New(isolate, _OPmat4SetRotateX));
	mat4->Set(String::NewFromUtf8(isolate, "SetRotY"), FunctionTemplate::New(isolate, _OPmat4SetRotateY));
	mat4->Set(String::NewFromUtf8(isolate, "SetRotZ"), FunctionTemplate::New(isolate, _OPmat4SetRotateZ));
	mat4->Set(String::NewFromUtf8(isolate, "SetVec3"), FunctionTemplate::New(isolate, _OPmat4SetVec3));
	mat4->Set(String::NewFromUtf8(isolate, "Identity"), FunctionTemplate::New(isolate, _OPmat4Identity));
	target->Set(String::NewFromUtf8(isolate, "mat4"), mat4);

	// OP.vec3
	Local<ObjectTemplate> vec3 = ObjectTemplate::New(isolate);
	vec3->Set(String::NewFromUtf8(isolate, "Create"), FunctionTemplate::New(isolate, _OPvec3Create));
	vec3->Set(String::NewFromUtf8(isolate, "FromPointer"), FunctionTemplate::New(isolate, _OPvec3CreateFromPointer));
	vec3->Set(String::NewFromUtf8(isolate, "Destroy"), FunctionTemplate::New(isolate, _OPvec3Destroy));
	vec3->Set(String::NewFromUtf8(isolate, "Set"), FunctionTemplate::New(isolate, _OPvec3Set));
	target->Set(String::NewFromUtf8(isolate, "vec3"), vec3);

}

#include "./Scripting/include/Scripting.h"
#include "./Math/include/Vector3.h"
#include "./Scripting/include/wrappers/V8Helper.h"

static void _OPvec3Create(const FunctionCallbackInfo<Value>& args) {
	OPvec3* vec = (OPvec3*)OPalloc(sizeof(OPvec3));
	if (args.Length() == 0){
		*vec = OPvec3Zero;
	}
	else {
		vec->x = args[0]->NumberValue();
		vec->y = args[1]->NumberValue();
		vec->z = args[2]->NumberValue();
	}

	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "Id"), Number::New(isolate, (i32)vec));
	obj->Set(String::NewFromUtf8(isolate, "X"), Function::New(isolate, _OPvec3X));
	obj->Set(String::NewFromUtf8(isolate, "Y"), Function::New(isolate, _OPvec3Y));
	obj->Set(String::NewFromUtf8(isolate, "Z"), Function::New(isolate, _OPvec3Z));
	obj->Set(String::NewFromUtf8(isolate, "XYZ"), Function::New(isolate, _OPvec3XYZ));
	obj->Set(String::NewFromUtf8(isolate, "Set"), Function::New(isolate, _OPvec3Set));
	obj->Set(String::NewFromUtf8(isolate, "Add"), Function::New(isolate, _OPvec3Add));
	obj->Set(String::NewFromUtf8(isolate, "Sub"), Function::New(isolate, _OPvec3Sub));

	return args.GetReturnValue().Set(obj);
}

static void _OPvec3CreateFromPointer(const FunctionCallbackInfo<Value>& args) {
	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "Id"), args[0]);
	obj->Set(String::NewFromUtf8(isolate, "X"), Function::New(isolate, _OPvec3X));
	obj->Set(String::NewFromUtf8(isolate, "Y"), Function::New(isolate, _OPvec3Y));
	obj->Set(String::NewFromUtf8(isolate, "Z"), Function::New(isolate, _OPvec3Z));
	obj->Set(String::NewFromUtf8(isolate, "XYZ"), Function::New(isolate, _OPvec3XYZ));
	obj->Set(String::NewFromUtf8(isolate, "Set"), Function::New(isolate, _OPvec3Set));
	obj->Set(String::NewFromUtf8(isolate, "Add"), Function::New(isolate, _OPvec3Add));
	obj->Set(String::NewFromUtf8(isolate, "Sub"), Function::New(isolate, _OPvec3Sub));

	return args.GetReturnValue().Set(obj);
}

static void _OPvec3Destroy(const FunctionCallbackInfo<Value>& args) {
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPfree(vec);
	
}

static void _OPvec3X(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	args.GetReturnValue().Set(Number::New(isolate, vec->x));
}

static void _OPvec3Y(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	return args.GetReturnValue().Set(Number::New(isolate, vec->y));
}

static void _OPvec3Z(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	return args.GetReturnValue().Set(Number::New(isolate, vec->z));
}

static void _OPvec3XYZ(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;

	Local<Array> tmp = Array::New(isolate);
	tmp->Set(0, Number::New(isolate, vec->x));
	tmp->Set(1, Number::New(isolate, vec->y));
	tmp->Set(2, Number::New(isolate, vec->z));

	return args.GetReturnValue().Set(tmp);
}

static void _OPvec3Set(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, &inScope, 4);
	if (inScope == -1) return;
	vec->x = args[0]->NumberValue();
	vec->y = args[1]->NumberValue();
	vec->z = args[2]->NumberValue();

	
}

static void _OPvec3Add(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vecA = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPvec3* vecB = (OPvec3*)args[1 - inScope]->ToObject()->Get(String::NewFromUtf8(isolate, "Id"))->Int32Value();
	vecA->x += vecB->x;
	vecA->y += vecB->y;
	vecA->z += vecB->z;

	
}

static void _OPvec3Sub(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPvec3* vecA = (OPvec3*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPvec3* vecB = (OPvec3*)args[1 - inScope]->Int32Value();
	vecA->x -= vecB->x;
	vecA->y -= vecB->y;
	vecA->z -= vecB->z;

	
}

#include "./Math/include/Matrix4.h"

static void _OPmat4Create(const FunctionCallbackInfo<Value>& args) {
	
	OPmat4* mat = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPmat4identity(mat);

	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "Id"), Number::New(isolate, (i32)mat));
	obj->Set(String::NewFromUtf8(isolate, "RotX"), Function::New(isolate, _OPmat4RotateX));
	obj->Set(String::NewFromUtf8(isolate, "RotY"), Function::New(isolate, _OPmat4RotateY));
	obj->Set(String::NewFromUtf8(isolate, "RotZ"), Function::New(isolate, _OPmat4RotateZ));
	obj->Set(String::NewFromUtf8(isolate, "SetRotX"), Function::New(isolate, _OPmat4SetRotateX));
	obj->Set(String::NewFromUtf8(isolate, "SetRotY"), Function::New(isolate, _OPmat4SetRotateY));
	obj->Set(String::NewFromUtf8(isolate, "SetRotZ"), Function::New(isolate, _OPmat4SetRotateZ));
	obj->Set(String::NewFromUtf8(isolate, "Identity"), Function::New(isolate, _OPmat4SetRotateZ));

	return args.GetReturnValue().Set(obj);
}

static void _OPmat4Destroy(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPfree(mat);
	
}

static void _OPmat4SetRotateY(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPmat4buildRotY(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4SetRotateX(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPmat4buildRotX(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4SetRotateZ(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1)  return;
	OPmat4buildRotZ(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4RotateY(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPmat4rotY(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4RotateX(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPmat4rotX(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4RotateZ(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1) return;
	OPmat4rotZ(mat, args[1 - inScope]->NumberValue());
	
}

static void _OPmat4SetVec3(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 2);
	if (inScope == -1)  return;
	OPmat4buildTranslate(mat, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
	
}

static void _OPmat4Identity(const FunctionCallbackInfo<Value>& args) {
	
	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, &inScope, 1);
	if (inScope == -1)  return;
	OPmat4identity(mat);
	
}

#endif