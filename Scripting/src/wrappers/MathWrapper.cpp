#include "../../include/wrappers/MathWrapper.h"

#ifdef OPIFEX_V8

#include "./Math/include/Vector3.h"
#include "./Math/include/Matrix4.h"

static V8Return _OPmat4Create(const V8Args& args);
static V8Return _OPmat4Destroy(const V8Args& args);
static V8Return _OPmat4RotateX(const V8Args& args);
static V8Return _OPmat4RotateY(const V8Args& args);
static V8Return _OPmat4RotateZ(const V8Args& args);
static V8Return _OPmat4Vec3(const V8Args& args);
static V8Return _OPmat4Scale(const V8Args& args);
static V8Return _OPmat4SetRotateX(const V8Args& args);
static V8Return _OPmat4SetRotateY(const V8Args& args);
static V8Return _OPmat4SetRotateZ(const V8Args& args);
static V8Return _OPmat4SetVec3(const V8Args& args);
static V8Return _OPmat4SetScale(const V8Args& args);
static V8Return _OPmat4Identity(const V8Args& args);

static V8Return _OPvec3Create(const V8Args& args);
static V8Return _OPvec3CreateFromPointer(const V8Args& args);
static V8Return _OPvec3Destroy(const V8Args& args);
static V8Return _OPvec3Set(const V8Args& args);
static V8Return _OPvec3X(const V8Args& args);
static V8Return _OPvec3Y(const V8Args& args);
static V8Return _OPvec3Z(const V8Args& args);
static V8Return _OPvec3XYZ(const V8Args& args);
static V8Return _OPvec3Add(const V8Args& args);
static V8Return _OPvec3Sub(const V8Args& args);

static V8Return _OPabs(const V8Args& args);
static V8Return _OPrand(const V8Args& args);
static V8Return _OPfloor(const V8Args& args);
static V8Return _OPpow(const V8Args& args);

void MathInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	// OP.mat4
	V8ObjectGlobal mat4 = CreateObjectG(isolate);
	SetFunctionG(isolate, mat4, "Create", _OPmat4Create);
	SetFunctionG(isolate, mat4, "Destroy", _OPmat4Destroy);
	SetFunctionG(isolate, mat4, "RotX", _OPmat4RotateX);
	SetFunctionG(isolate, mat4, "RotY", _OPmat4RotateY);
	SetFunctionG(isolate, mat4, "RotZ", _OPmat4RotateZ);
	SetFunctionG(isolate, mat4, "Vec3", _OPmat4Vec3);
	SetFunctionG(isolate, mat4, "SetRotX", _OPmat4SetRotateX);
	SetFunctionG(isolate, mat4, "SetRotY", _OPmat4SetRotateY);
	SetFunctionG(isolate, mat4, "SetRotZ", _OPmat4SetRotateZ);
	SetFunctionG(isolate, mat4, "SetVec3", _OPmat4SetVec3);
	SetFunctionG(isolate, mat4, "Identity", _OPmat4Identity);
	SetObjectG(isolate, target, "mat4", mat4);

	// OP.vec3
	V8ObjectGlobal vec3 = CreateObjectG(isolate);
	SetFunctionG(isolate, vec3, "Create", _OPvec3Create);
	SetFunctionG(isolate, vec3, "FromPointer", _OPvec3CreateFromPointer);
	SetFunctionG(isolate, vec3, "Destroy", _OPvec3Destroy);
	SetFunctionG(isolate, vec3, "Set", _OPvec3Set);
	SetObjectG(isolate, target, "vec3", vec3);


	// OP.math
	V8ObjectGlobal math = CreateObjectG(isolate);
	SetFunctionG(isolate, math, "Abs", _OPabs);
	SetFunctionG(isolate, math, "Rand", _OPrand);
	SetFunctionG(isolate, math, "Floor", _OPfloor);
	SetFunctionG(isolate, math, "Pow", _OPpow);
	SetValueG(isolate, math, "PI", GetNumberF32(isolate, OPpi));
	SetObjectG(isolate, target, "math", math);

}

void MathInitializeMethodsO(V8isolate* isolate, V8Object target) {

	// OP.mat4
	V8Object mat4 = CreateObject(isolate);
	SetFunction(isolate, mat4, "Create", _OPmat4Create);
	SetFunction(isolate, mat4, "Destroy", _OPmat4Destroy);
	SetFunction(isolate, mat4, "RotX", _OPmat4RotateX);
	SetFunction(isolate, mat4, "RotY", _OPmat4RotateY);
	SetFunction(isolate, mat4, "RotZ", _OPmat4RotateZ);
	SetFunction(isolate, mat4, "Vec3", _OPmat4Vec3);
	SetFunction(isolate, mat4, "SetRotX", _OPmat4SetRotateX);
	SetFunction(isolate, mat4, "SetRotY", _OPmat4SetRotateY);
	SetFunction(isolate, mat4, "SetRotZ", _OPmat4SetRotateZ);
	SetFunction(isolate, mat4, "SetVec3", _OPmat4SetVec3);
	SetFunction(isolate, mat4, "Identity", _OPmat4Identity);
	SetObject(isolate, target, "mat4", mat4);

	// OP.vec3
	V8Object vec3 = CreateObject(isolate);
	SetFunction(isolate, vec3, "Create", _OPvec3Create);
	SetFunction(isolate, vec3, "FromPointer", _OPvec3CreateFromPointer);
	SetFunction(isolate, vec3, "Destroy", _OPvec3Destroy);
	SetFunction(isolate, vec3, "Set", _OPvec3Set);
	SetObject(isolate, target, "vec3", vec3);


	// OP.math
	V8Object math = CreateObject(isolate);
	SetFunction(isolate, math, "Abs", _OPabs);
	SetFunction(isolate, math, "Rand", _OPrand);
	SetFunction(isolate, math, "Floor", _OPfloor);
	SetFunction(isolate, math, "Pow", _OPpow);
	SetValue(isolate, math, "PI", GetNumberF32(isolate, OPpi));
	SetObject(isolate, target, "math", math);

}

static V8Return _OPabs(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumber(isolate, OPabs(args[0]->NumberValue())));
}

static V8Return _OPpow(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumber(isolate, OPpow(args[0]->NumberValue(), args[1]->NumberValue())));
}

static V8Return _OPfloor(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumber(isolate, OPfloor(args[0]->NumberValue())));
}

static V8Return _OPrand(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPrandom()));
}

static V8Return _OPvec3Create(const V8Args& args) {
	V8Scope scope;

	OPvec3* vec = (OPvec3*)OPalloc(sizeof(OPvec3));
	if (args.Length() == 0){
		*vec = OPvec3Zero;
	}
	else {
		vec->x = args[0]->NumberValue();
		vec->y = args[1]->NumberValue();
		vec->z = args[2]->NumberValue();
	}

	V8Object obj = CreateTypedObject(isolate, vec, OPscript_VEC3);
	SetFunction(isolate, obj, "X", _OPvec3X);
	SetFunction(isolate, obj, "Y", _OPvec3Y);
	SetFunction(isolate, obj, "Z", _OPvec3Z);
	SetFunction(isolate, obj, "XYZ", _OPvec3XYZ);
	SetFunction(isolate, obj, "Set", _OPvec3Set);
	SetFunction(isolate, obj, "Add", _OPvec3Add);
	SetFunction(isolate, obj, "Sub", _OPvec3Sub);

	return SetReturn(args, &scope, obj);
}

static V8Return _OPvec3CreateFromPointer(const V8Args& args) {
	V8Scope scope;

	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", args[0]);
	SetFunction(isolate, obj, "X", _OPvec3X);
	SetFunction(isolate, obj, "Y", _OPvec3Y);
	SetFunction(isolate, obj, "Z", _OPvec3Z);
	SetFunction(isolate, obj, "XYZ", _OPvec3XYZ);
	SetFunction(isolate, obj, "Set", _OPvec3Set);
	SetFunction(isolate, obj, "Add", _OPvec3Add);
	SetFunction(isolate, obj, "Sub", _OPvec3Sub);

	return SetReturn(args, &scope, obj);
}

static V8Return _OPvec3Destroy(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPfree(vec);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPvec3X(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	return SetReturn(args, &scope, GetNumberF32(isolate, vec->x));
}

static V8Return _OPvec3Y(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	return SetReturn(args, &scope, GetNumberF32(isolate, vec->y));
}

static V8Return _OPvec3Z(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	return SetReturn(args, &scope, GetNumberF32(isolate, vec->z));
}

static V8Return _OPvec3XYZ(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	Handle<Array> tmp = CreateArray(isolate);
	tmp->Set(0, GetNumber(isolate, vec->x));
	tmp->Set(1, GetNumber(isolate, vec->y));
	tmp->Set(2, GetNumber(isolate, vec->z));

	return SetReturn(args, &scope, tmp);
}

static V8Return _OPvec3Set(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vec = (OPvec3*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	vec->x = args[1 - inScope]->NumberValue();
	vec->y = args[2 - inScope]->NumberValue();
	vec->z = args[3 - inScope]->NumberValue();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPvec3Add(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vecA = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPvec3* vecB = (OPvec3*)args[1 - inScope]->ToObject()->Get(GetString(isolate, "Id"))->Int32Value();
	vecA->x += vecB->x;
	vecA->y += vecB->y;
	vecA->z += vecB->z;

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPvec3Sub(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPvec3* vecA = (OPvec3*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPvec3* vecB = (OPvec3*)args[1 - inScope]->Int32Value();
	vecA->x -= vecB->x;
	vecA->y -= vecB->y;
	vecA->z -= vecB->z;

	return SetReturn(args, &scope, GetNull(isolate));
}


static V8Return _OPmat4Create(const V8Args& args) {
	V8Scope scope;

	OPmat4* mat = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPmat4identity(mat);

	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", GetNumber(isolate, (i32)mat));
	SetFunction(isolate, obj, "RotX", _OPmat4RotateX);
	SetFunction(isolate, obj, "RotY", _OPmat4RotateY);
	SetFunction(isolate, obj, "RotZ", _OPmat4RotateZ);
	SetFunction(isolate, obj, "Vec3", _OPmat4Vec3);
	SetFunction(isolate, obj, "Scale", _OPmat4Scale);
	SetFunction(isolate, obj, "SetRotX", _OPmat4SetRotateX);
	SetFunction(isolate, obj, "SetRotY", _OPmat4SetRotateY);
	SetFunction(isolate, obj, "SetRotZ", _OPmat4SetRotateZ);
	SetFunction(isolate, obj, "SetVec3", _OPmat4SetVec3);
	SetFunction(isolate, obj, "SetScale", _OPmat4SetScale);
	SetFunction(isolate, obj, "Identity", _OPmat4Identity);

	return SetReturn(args, &scope, obj);
}

static V8Return _OPmat4Destroy(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPfree(mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetRotateY(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4buildRotY(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetRotateX(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4buildRotX(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetRotateZ(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));
	OPmat4buildRotZ(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateY(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4rotY(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateX(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4rotX(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateZ(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4rotZ(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Vec3(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4translate(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Scale(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4scl(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetVec3(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));
	OPmat4buildTranslate(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetScale(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));

	OPmat4buildScl(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Identity(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 1);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4identity(mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

#endif