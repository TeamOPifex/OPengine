#include "../../include/wrappers/MathWrapper.h"

#ifdef OPIFEX_V8

#include "./Math/include/Vector3.h"
#include "./Math/include/Matrix4.h"
#include "./Math/include/Tweening.h"

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
static V8Return _OPmat4Multiply(const V8Args& args);

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

static V8Return _OPtweenLinear(const V8Args& args);
static V8Return _OPtweenQuadraticEaseIn(const V8Args& args);
static V8Return _OPtweenQuadraticEaseOut(const V8Args& args);
static V8Return _OPtweenQuadraticEaseInOut(const V8Args& args);
static V8Return _OPtweenCubicEaseIn(const V8Args& args);
static V8Return _OPtweenCubicEaseOut(const V8Args& args);
static V8Return _OPtweenCubicEaseInOut(const V8Args& args);
static V8Return _OPtweenQuarticEaseIn(const V8Args& args);
static V8Return _OPtweenQuarticEaseOut(const V8Args& args);
static V8Return _OPtweenQuarticEaseInOut(const V8Args& args);
static V8Return _OPtweenQuinticEaseIn(const V8Args& args);
static V8Return _OPtweenQuinticEaseOut(const V8Args& args);
static V8Return _OPtweenQuinticEaseInOut(const V8Args& args);
static V8Return _OPtweenSineEaseIn(const V8Args& args);
static V8Return _OPtweenSineEaseOut(const V8Args& args);
static V8Return _OPtweenSineEaseInOut(const V8Args& args);
static V8Return _OPtweenCircularEaseIn(const V8Args& args);
static V8Return _OPtweenCircularEaseOut(const V8Args& args);
static V8Return _OPtweenCircularEaseInOut(const V8Args& args);
static V8Return _OPtweenExponentialEaseIn(const V8Args& args);
static V8Return _OPtweenExponentialEaseOut(const V8Args& args);
static V8Return _OPtweenExponentialEaseInOut(const V8Args& args);
static V8Return _OPtweenElasticEaseIn(const V8Args& args);
static V8Return _OPtweenElasticEaseOut(const V8Args& args);
static V8Return _OPtweenElasticEaseInOut(const V8Args& args);
static V8Return _OPtweenBackEaseIn(const V8Args& args);
static V8Return _OPtweenBackEaseOut(const V8Args& args);
static V8Return _OPtweenBackEaseInOut(const V8Args& args);
static V8Return _OPtweenBounceEaseOut(const V8Args& args);
static V8Return _OPtweenBounceEaseIn(const V8Args& args);
static V8Return _OPtweenBounceEaseInOut(const V8Args& args);

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
	SetFunction(isolate, mat4, "Mul", _OPmat4Multiply);
	SetObject(isolate, target, "mat4", mat4);

	// OP.vec3
	V8Object vec3 = CreateObject(isolate);
	SetFunction(isolate, vec3, "Create", _OPvec3Create);
	SetFunction(isolate, vec3, "FromPointer", _OPvec3CreateFromPointer);
	SetFunction(isolate, vec3, "Destroy", _OPvec3Destroy);
	SetFunction(isolate, vec3, "Set", _OPvec3Set);
	SetFunction(isolate, vec3, "Add", _OPvec3Add);
	SetObject(isolate, target, "vec3", vec3);


	// OP.math
	V8Object math = CreateObject(isolate);
	SetFunction(isolate, math, "Abs", _OPabs);
	SetFunction(isolate, math, "Rand", _OPrand);
	SetFunction(isolate, math, "Floor", _OPfloor);
	SetFunction(isolate, math, "Pow", _OPpow);
	SetValue(isolate, math, "PI", GetNumberF32(isolate, OPpi));
	SetObject(isolate, target, "math", math);

	// OP.tween
	V8Object tween = CreateObject(isolate);

	SetFunction(isolate, tween, "Linear", _OPtweenLinear);
	SetFunction(isolate, tween, "QuadraticEaseIn", _OPtweenQuadraticEaseIn);
	SetFunction(isolate, tween, "QuadraticEaseOut", _OPtweenQuadraticEaseOut);
	SetFunction(isolate, tween, "QuadraticEaseInOut", _OPtweenQuadraticEaseInOut);
	SetFunction(isolate, tween, "CubicEaseIn", _OPtweenCubicEaseIn);
	SetFunction(isolate, tween, "CubicEaseOut", _OPtweenCubicEaseOut);
	SetFunction(isolate, tween, "CubicEaseInOut", _OPtweenCubicEaseInOut);
	SetFunction(isolate, tween, "QuarticEaseIn", _OPtweenQuarticEaseIn);
	SetFunction(isolate, tween, "QuarticEaseOut", _OPtweenQuarticEaseOut);
	SetFunction(isolate, tween, "QuarticEaseInOut", _OPtweenQuarticEaseInOut);
	SetFunction(isolate, tween, "QuinticEaseIn", _OPtweenQuinticEaseIn);
	SetFunction(isolate, tween, "QuinticEaseOut", _OPtweenQuinticEaseOut);
	SetFunction(isolate, tween, "QuinticEaseInOut", _OPtweenQuinticEaseInOut);
	SetFunction(isolate, tween, "SineEaseIn", _OPtweenSineEaseIn);
	SetFunction(isolate, tween, "SineEaseOut", _OPtweenSineEaseOut);
	SetFunction(isolate, tween, "SineEaseInOut", _OPtweenSineEaseInOut);
	SetFunction(isolate, tween, "CircularEaseIn", _OPtweenCircularEaseIn);
	SetFunction(isolate, tween, "CircularEaseOut", _OPtweenCircularEaseOut);
	SetFunction(isolate, tween, "CircularEaseInOut", _OPtweenCircularEaseInOut);
	SetFunction(isolate, tween, "ExponentialEaseIn", _OPtweenExponentialEaseIn);
	SetFunction(isolate, tween, "ExponentialEaseOut", _OPtweenExponentialEaseOut);
	SetFunction(isolate, tween, "ExponentialEaseInOut", _OPtweenExponentialEaseInOut);
	SetFunction(isolate, tween, "ElasticEaseIn", _OPtweenElasticEaseIn);
	SetFunction(isolate, tween, "ElasticEaseOut", _OPtweenElasticEaseOut);
	SetFunction(isolate, tween, "ElasticEaseInOut", _OPtweenElasticEaseInOut);
	SetFunction(isolate, tween, "BackEaseIn", _OPtweenBackEaseIn);
	SetFunction(isolate, tween, "BackEaseOut", _OPtweenBackEaseOut);
	SetFunction(isolate, tween, "BackEaseInOut", _OPtweenBackEaseInOut);
	SetFunction(isolate, tween, "BounceEaseOut", _OPtweenBounceEaseOut);
	SetFunction(isolate, tween, "BounceEaseIn", _OPtweenBounceEaseIn);
	SetFunction(isolate, tween, "BounceEaseInOut", _OPtweenBounceEaseInOut);
	SetObject(isolate, target, "tween", tween);

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

	if (args.Length() == 0) {
		*vec = OPvec3Zero;
	} else {
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
	OPmat4Identity(mat);


	V8Object obj = CreateTypedObject(isolate, mat, OPscript_MAT4);
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
	OPmat4BuildRotY(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetRotateX(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4BuildRotX(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetRotateZ(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));
	OPmat4BuildRotZ(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateY(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4RotY(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateX(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4RotX(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4RotateZ(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4RotZ(mat, args[1 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Vec3(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4Translate(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Scale(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4Scl(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetVec3(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));
	OPmat4BuildTranslate(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4SetScale(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 4);
	if (inScope == -1)  SetReturn(args, &scope, GetNull(isolate));

	OPmat4BuildScl(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Identity(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPmat4* mat = (OPmat4*)GetPointer(args, isolate, &inScope, 1);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPmat4Identity(mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OPmat4Multiply(const V8Args& args) {
	V8Scope scope;

	OPmat4* dest = (OPmat4*)GetArgPointer(args, isolate, 0);
	OPmat4* a = (OPmat4*)GetArgPointer(args, isolate, 1);
	OPmat4* b = (OPmat4*)GetArgPointer(args, isolate, 2);

	OPmat4Mul(dest, a, b);

	return SetReturn(args, &scope, GetNull(isolate));
}



static V8Return _OPtweenLinear(const V8Args& args) {
	V8Scope scope;
	OPfloat val = args[0]->NumberValue();
	OPfloat result = OPtween_linear(val);
	OPlog("Tween Linear: %f, %f", val, result);
	return SetReturn(args, &scope, GetNumberF32(isolate, result));
}

static V8Return _OPtweenQuadraticEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quadraticEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenQuadraticEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quadraticEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenQuadraticEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quadraticEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenCubicEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_cubicEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenCubicEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_cubicEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenCubicEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_cubicEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenQuarticEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quarticEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenQuarticEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quarticEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenQuarticEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quarticEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenQuinticEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quinticEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenQuinticEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quinticEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenQuinticEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_quinticEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenSineEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_sineEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenSineEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_sineEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenSineEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_sineEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenCircularEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_circularEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenCircularEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_circularEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenCircularEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_circularEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenExponentialEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_exponentialEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenExponentialEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_exponentialEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenExponentialEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_exponentialEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenElasticEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_elasticEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenElasticEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_elasticEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenElasticEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_elasticEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenBackEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_backEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenBackEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_backEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenBackEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_backEaseInOut(args[0]->NumberValue())));
}
static V8Return _OPtweenBounceEaseOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_bounceEaseOut(args[0]->NumberValue())));
}
static V8Return _OPtweenBounceEaseIn(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_bounceEaseIn(args[0]->NumberValue())));
}
static V8Return _OPtweenBounceEaseInOut(const V8Args& args) {
	V8Scope scope;	
	return SetReturn(args, &scope, GetNumberF32(isolate, OPtween_bounceEaseInOut(args[0]->NumberValue())));
}


#endif