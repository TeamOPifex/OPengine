
// #include "../../include/wrappers/MathWrapper.h"




// #include "../../include/wrappers/MathWrapper.h"

// #ifdef OPIFEX_OPTION_V8

// #include "./Math/include/OPvec3.h"
// #include "./Math/include/OPmat4.h"
// #include "./Math/include/OPtween.h"

// static V8Return _OPmat4Create(const V8Args& args);
// static V8Return _OPmat4Destroy(const V8Args& args);
// static V8Return _OPmat4RotateX(const V8Args& args);
// static V8Return _OPmat4RotateY(const V8Args& args);
// static V8Return _OPmat4RotateZ(const V8Args& args);
// static V8Return _OPmat4Vec3(const V8Args& args);
// static V8Return _OPmat4Scale(const V8Args& args);
// static V8Return _OPmat4SetRotateX(const V8Args& args);
// static V8Return _OPmat4SetRotateY(const V8Args& args);
// static V8Return _OPmat4SetRotateZ(const V8Args& args);
// static V8Return _OPmat4SetVec3(const V8Args& args);
// static V8Return _OPmat4SetScale(const V8Args& args);
// static V8Return _OPmat4Identity(const V8Args& args);
// static V8Return _OPmat4Multiply(const V8Args& args);

// static V8Return _OPvec3Create(const V8Args& args);
// static V8Return _OPvec3CreateFromPointer(const V8Args& args);
// static V8Return _OPvec3Destroy(const V8Args& args);
// static V8Return _OPvec3Set(const V8Args& args);
// static V8Return _OPvec3X(const V8Args& args);
// static V8Return _OPvec3Y(const V8Args& args);
// static V8Return _OPvec3Z(const V8Args& args);
// static V8Return _OPvec3XYZ(const V8Args& args);
// static V8Return _OPvec3Add(const V8Args& args);
// static V8Return _OPvec3Sub(const V8Args& args);

// static V8Return _OPabs(const V8Args& args);
// static V8Return _OPrand(const V8Args& args);
// static V8Return _OPfloor(const V8Args& args);
// static V8Return _OPpow(const V8Args& args);

// static V8Return _OPtweenLinear(const V8Args& args);
// static V8Return _OPtweenQuadraticEaseIn(const V8Args& args);
// static V8Return _OPtweenQuadraticEaseOut(const V8Args& args);
// static V8Return _OPtweenQuadraticEaseInOut(const V8Args& args);
// static V8Return _OPtweenCubicEaseIn(const V8Args& args);
// static V8Return _OPtweenCubicEaseOut(const V8Args& args);
// static V8Return _OPtweenCubicEaseInOut(const V8Args& args);
// static V8Return _OPtweenQuarticEaseIn(const V8Args& args);
// static V8Return _OPtweenQuarticEaseOut(const V8Args& args);
// static V8Return _OPtweenQuarticEaseInOut(const V8Args& args);
// static V8Return _OPtweenQuinticEaseIn(const V8Args& args);
// static V8Return _OPtweenQuinticEaseOut(const V8Args& args);
// static V8Return _OPtweenQuinticEaseInOut(const V8Args& args);
// static V8Return _OPtweenSineEaseIn(const V8Args& args);
// static V8Return _OPtweenSineEaseOut(const V8Args& args);
// static V8Return _OPtweenSineEaseInOut(const V8Args& args);
// static V8Return _OPtweenCircularEaseIn(const V8Args& args);
// static V8Return _OPtweenCircularEaseOut(const V8Args& args);
// static V8Return _OPtweenCircularEaseInOut(const V8Args& args);
// static V8Return _OPtweenExponentialEaseIn(const V8Args& args);
// static V8Return _OPtweenExponentialEaseOut(const V8Args& args);
// static V8Return _OPtweenExponentialEaseInOut(const V8Args& args);
// static V8Return _OPtweenElasticEaseIn(const V8Args& args);
// static V8Return _OPtweenElasticEaseOut(const V8Args& args);
// static V8Return _OPtweenElasticEaseInOut(const V8Args& args);
// static V8Return _OPtweenBackEaseIn(const V8Args& args);
// static V8Return _OPtweenBackEaseOut(const V8Args& args);
// static V8Return _OPtweenBackEaseInOut(const V8Args& args);
// static V8Return _OPtweenBounceEaseOut(const V8Args& args);
// static V8Return _OPtweenBounceEaseIn(const V8Args& args);
// static V8Return _OPtweenBounceEaseInOut(const V8Args& args);

// void MathInitializeMethodsO(V8isolate* isolate, V8Object target) {

// 	// OP.mat4
// 	V8Object mat4 = OPscriptV8CreateObject(isolate);
// 	OPscriptV8SetFunction(isolate, mat4, "Create", _OPmat4Create);
// 	OPscriptV8SetFunction(isolate, mat4, "Destroy", _OPmat4Destroy);
// 	OPscriptV8SetFunction(isolate, mat4, "RotX", _OPmat4RotateX);
// 	OPscriptV8SetFunction(isolate, mat4, "RotY", _OPmat4RotateY);
// 	OPscriptV8SetFunction(isolate, mat4, "RotZ", _OPmat4RotateZ);
// 	OPscriptV8SetFunction(isolate, mat4, "Vec3", _OPmat4Vec3);
// 	OPscriptV8SetFunction(isolate, mat4, "SetRotX", _OPmat4SetRotateX);
// 	OPscriptV8SetFunction(isolate, mat4, "SetRotY", _OPmat4SetRotateY);
// 	OPscriptV8SetFunction(isolate, mat4, "SetRotZ", _OPmat4SetRotateZ);
// 	OPscriptV8SetFunction(isolate, mat4, "SetVec3", _OPmat4SetVec3);
// 	OPscriptV8SetFunction(isolate, mat4, "Identity", _OPmat4Identity);
// 	OPscriptV8SetFunction(isolate, mat4, "Mul", _OPmat4Multiply);
// 	OPscriptV8SetObject(isolate, target, "mat4", mat4);

// 	// OP.vec3
// 	V8Object vec3 = OPscriptV8CreateObject(isolate);
// 	OPscriptV8SetFunction(isolate, vec3, "Create", _OPvec3Create);
// 	OPscriptV8SetFunction(isolate, vec3, "FromPointer", _OPvec3CreateFromPointer);
// 	OPscriptV8SetFunction(isolate, vec3, "Destroy", _OPvec3Destroy);
// 	OPscriptV8SetFunction(isolate, vec3, "Set", _OPvec3Set);
// 	OPscriptV8SetFunction(isolate, vec3, "Add", _OPvec3Add);
// 	OPscriptV8SetObject(isolate, target, "vec3", vec3);


// 	// OP.math
// 	V8Object math = OPscriptV8CreateObject(isolate);
// 	OPscriptV8SetFunction(isolate, math, "Abs", _OPabs);
// 	OPscriptV8SetFunction(isolate, math, "Rand", _OPrand);
// 	OPscriptV8SetFunction(isolate, math, "Floor", _OPfloor);
// 	OPscriptV8SetFunction(isolate, math, "Pow", _OPpow);
// 	OPscriptV8SetValue(isolate, math, "PI", OPscriptV8GetNumberF32(isolate, OPpi));
// 	OPscriptV8SetObject(isolate, target, "math", math);

// 	// OP.tween
// 	V8Object tween = OPscriptV8CreateObject(isolate);

// 	OPscriptV8SetFunction(isolate, tween, "Linear", _OPtweenLinear);
// 	OPscriptV8SetFunction(isolate, tween, "QuadraticEaseIn", _OPtweenQuadraticEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "QuadraticEaseOut", _OPtweenQuadraticEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "QuadraticEaseInOut", _OPtweenQuadraticEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "CubicEaseIn", _OPtweenCubicEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "CubicEaseOut", _OPtweenCubicEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "CubicEaseInOut", _OPtweenCubicEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "QuarticEaseIn", _OPtweenQuarticEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "QuarticEaseOut", _OPtweenQuarticEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "QuarticEaseInOut", _OPtweenQuarticEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "QuinticEaseIn", _OPtweenQuinticEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "QuinticEaseOut", _OPtweenQuinticEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "QuinticEaseInOut", _OPtweenQuinticEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "SineEaseIn", _OPtweenSineEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "SineEaseOut", _OPtweenSineEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "SineEaseInOut", _OPtweenSineEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "CircularEaseIn", _OPtweenCircularEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "CircularEaseOut", _OPtweenCircularEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "CircularEaseInOut", _OPtweenCircularEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "ExponentialEaseIn", _OPtweenExponentialEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "ExponentialEaseOut", _OPtweenExponentialEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "ExponentialEaseInOut", _OPtweenExponentialEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "ElasticEaseIn", _OPtweenElasticEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "ElasticEaseOut", _OPtweenElasticEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "ElasticEaseInOut", _OPtweenElasticEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "BackEaseIn", _OPtweenBackEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "BackEaseOut", _OPtweenBackEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "BackEaseInOut", _OPtweenBackEaseInOut);
// 	OPscriptV8SetFunction(isolate, tween, "BounceEaseOut", _OPtweenBounceEaseOut);
// 	OPscriptV8SetFunction(isolate, tween, "BounceEaseIn", _OPtweenBounceEaseIn);
// 	OPscriptV8SetFunction(isolate, tween, "BounceEaseInOut", _OPtweenBounceEaseInOut);
// 	OPscriptV8SetObject(isolate, target, "tween", tween);

// }

// static V8Return _OPabs(const V8Args& args) {
// 	V8Scope scope;
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPabs(args[0]->NumberValue())));
// }

// static V8Return _OPpow(const V8Args& args) {
// 	V8Scope scope;
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPpow(args[0]->NumberValue(), args[1]->NumberValue())));
// }

// static V8Return _OPfloor(const V8Args& args) {
// 	V8Scope scope;
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPfloor(args[0]->NumberValue())));
// }

// static V8Return _OPrand(const V8Args& args) {
// 	V8Scope scope;
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPrandom()));
// }

// V8Object _vec3Setup(V8Object obj, OPvec3* val) {

// 	obj->Set(OPscriptV8GetString(isolate, "Id"), OPscriptV8GetNumber(isolate, (OPint)val));
// 	obj->Set(OPscriptV8GetString(isolate, "Type"), OPscriptV8GetNumber(isolate, (OPint)OPscript_VEC3));

// 	OPscriptV8SetFunction(isolate, obj, "X", _OPvec3X);
// 	OPscriptV8SetFunction(isolate, obj, "Y", _OPvec3Y);
// 	OPscriptV8SetFunction(isolate, obj, "Z", _OPvec3Z);
// 	OPscriptV8SetFunction(isolate, obj, "XYZ", _OPvec3XYZ);
// 	OPscriptV8SetFunction(isolate, obj, "Set", _OPvec3Set);
// 	OPscriptV8SetFunction(isolate, obj, "Add", _OPvec3Add);
// 	OPscriptV8SetFunction(isolate, obj, "Sub", _OPvec3Sub);

// 	return obj;
// }

// OPscriptObjectPersistent OPscriptCreatePersistent(OPvec3* val) {
// 	OPscriptObjectPersistent result = OPscriptGetObj();
// 	V8Object obj = Local<Object>::New(isolate, result);
// 	//V8Object obj = V8Object::New(isolate, result);
// 	_vec3Setup(obj, val);
// 	return result;
// }

// V8Object OPscriptCreate(OPvec3* val) {
// 	V8Object obj = OPscriptV8CreateObject(isolate);
// 	_vec3Setup(obj, val);
// 	return obj;
// }

// static V8Return _OPvec3Create(const V8Args& args) {
// 	V8Scope scope;

// 	OPvec3* vec = (OPvec3*)OPalloc(sizeof(OPvec3));

// 	if (args.Length() == 0) {
// 		*vec = OPVEC3_ZERO;
// 	} else {
// 		vec->x = args[0]->NumberValue();
// 		vec->y = args[1]->NumberValue();
// 		vec->z = args[2]->NumberValue();
// 	}
	

// 	V8Object obj = OPscriptV8CreateObject(isolate);
// 	_vec3Setup(obj, vec);
	
	
// 	//OPscriptV8SetFunction(isolate, obj, "X", _OPvec3X);
// 	//OPscriptV8SetFunction(isolate, obj, "Y", _OPvec3Y);
// 	//OPscriptV8SetFunction(isolate, obj, "Z", _OPvec3Z);
// 	//OPscriptV8SetFunction(isolate, obj, "XYZ", _OPvec3XYZ);
// 	//OPscriptV8SetFunction(isolate, obj, "Set", _OPvec3Set);
// 	//OPscriptV8SetFunction(isolate, obj, "Add", _OPvec3Add);
// 	//OPscriptV8SetFunction(isolate, obj, "Sub", _OPvec3Sub);

// 	return OPscriptV8SetReturn(args, &scope, obj);
// }

// static V8Return _OPvec3CreateFromPointer(const V8Args& args) {
// 	V8Scope scope;

// 	V8Object obj = OPscriptV8CreateObject(isolate);
// 	OPscriptV8SetValue(isolate, obj, "Id", args[0]);
// 	OPscriptV8SetFunction(isolate, obj, "X", _OPvec3X);
// 	OPscriptV8SetFunction(isolate, obj, "Y", _OPvec3Y);
// 	OPscriptV8SetFunction(isolate, obj, "Z", _OPvec3Z);
// 	OPscriptV8SetFunction(isolate, obj, "XYZ", _OPvec3XYZ);
// 	OPscriptV8SetFunction(isolate, obj, "Set", _OPvec3Set);
// 	OPscriptV8SetFunction(isolate, obj, "Add", _OPvec3Add);
// 	OPscriptV8SetFunction(isolate, obj, "Sub", _OPvec3Sub);

// 	return OPscriptV8SetReturn(args, &scope, obj);
// }

// static V8Return _OPvec3Destroy(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPfree(vec);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPvec3X(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, vec->x));
// }

// static V8Return _OPvec3Y(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, vec->y));
// }

// static V8Return _OPvec3Z(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, vec->z));
// }

// static V8Return _OPvec3XYZ(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

// 	Handle<Array> tmp = OPscriptV8CreateArray(isolate);
// 	tmp->Set(0, OPscriptV8GetNumber(isolate, vec->x));
// 	tmp->Set(1, OPscriptV8GetNumber(isolate, vec->y));
// 	tmp->Set(2, OPscriptV8GetNumber(isolate, vec->z));

// 	return OPscriptV8SetReturn(args, &scope, tmp);
// }

// static V8Return _OPvec3Set(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vec = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 4);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	vec->x = args[1 - inScope]->NumberValue();
// 	vec->y = args[2 - inScope]->NumberValue();
// 	vec->z = args[3 - inScope]->NumberValue();

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPvec3Add(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vecA = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPvec3* vecB = (OPvec3*)args[1 - inScope]->ToObject()->Get(OPscriptV8GetString(isolate, "Id"))->Int32Value();
// 	vecA->x += vecB->x;
// 	vecA->y += vecB->y;
// 	vecA->z += vecB->z;

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPvec3Sub(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPvec3* vecA = (OPvec3*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPvec3* vecB = (OPvec3*)args[1 - inScope]->Int32Value();
// 	vecA->x -= vecB->x;
// 	vecA->y -= vecB->y;
// 	vecA->z -= vecB->z;

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }


// static V8Return _OPmat4Create(const V8Args& args) {
// 	V8Scope scope;

// 	OPmat4* mat = (OPmat4*)OPalloc(sizeof(OPmat4));
// 	OPmat4Identity(mat);


// 	V8Object obj = OPscriptV8CreateTypedObject(isolate, mat, OPscript_MAT4);
// 	OPscriptV8SetFunction(isolate, obj, "RotX", _OPmat4RotateX);
// 	OPscriptV8SetFunction(isolate, obj, "RotY", _OPmat4RotateY);
// 	OPscriptV8SetFunction(isolate, obj, "RotZ", _OPmat4RotateZ);
// 	OPscriptV8SetFunction(isolate, obj, "Vec3", _OPmat4Vec3);
// 	OPscriptV8SetFunction(isolate, obj, "Scale", _OPmat4Scale);
// 	OPscriptV8SetFunction(isolate, obj, "SetRotX", _OPmat4SetRotateX);
// 	OPscriptV8SetFunction(isolate, obj, "SetRotY", _OPmat4SetRotateY);
// 	OPscriptV8SetFunction(isolate, obj, "SetRotZ", _OPmat4SetRotateZ);
// 	OPscriptV8SetFunction(isolate, obj, "SetVec3", _OPmat4SetVec3);
// 	OPscriptV8SetFunction(isolate, obj, "SetScale", _OPmat4SetScale);
// 	OPscriptV8SetFunction(isolate, obj, "Identity", _OPmat4Identity);

// 	return OPscriptV8SetReturn(args, &scope, obj);
// }

// static V8Return _OPmat4Destroy(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPfree(mat);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4SetRotateY(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	*mat = OPmat4RotY(args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4SetRotateX(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	*mat = OPmat4RotX(args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4SetRotateZ(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1)  OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	*mat = OPmat4RotZ(args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4RotateY(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4RotY(mat, args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4RotateX(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4RotX(mat, args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4RotateZ(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4RotZ(mat, args[1 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4Vec3(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 4);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4Translate(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4Scale(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 4);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4Scl(mat, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4SetVec3(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 4);
// 	if (inScope == -1)  OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	*mat = OPmat4Translate(args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4SetScale(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 4);
// 	if (inScope == -1)  OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

// 	*mat = OPmat4Scl(args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue());

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4Identity(const V8Args& args) {
// 	V8Scope scope;

// 	i32 inScope;
// 	OPmat4* mat = (OPmat4*)OPscriptV8GetPointer(args, isolate, &inScope, 1);
// 	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// 	OPmat4Identity(mat);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _OPmat4Multiply(const V8Args& args) {
// 	V8Scope scope;

// 	OPmat4* dest = (OPmat4*)OPscriptV8GetArgPointer(args, isolate, 0);
// 	OPmat4* a = (OPmat4*)OPscriptV8GetArgPointer(args, isolate, 1);
// 	OPmat4* b = (OPmat4*)OPscriptV8GetArgPointer(args, isolate, 2);
// 	*dest = (*a) * (*b);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }



// static V8Return _OPtweenLinear(const V8Args& args) {
// 	V8Scope scope;
// 	OPfloat val = args[0]->NumberValue();
// 	OPfloat result = OPtween_linear(val);
// 	OPlog("Tween Linear: %f, %f", val, result);
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, result));
// }

// static V8Return _OPtweenQuadraticEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quadraticEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuadraticEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quadraticEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuadraticEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quadraticEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCubicEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_cubicEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCubicEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_cubicEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCubicEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_cubicEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuarticEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quarticEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuarticEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quarticEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuarticEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quarticEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuinticEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quinticEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuinticEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quinticEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenQuinticEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_quinticEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenSineEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_sineEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenSineEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_sineEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenSineEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_sineEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCircularEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_circularEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCircularEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_circularEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenCircularEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_circularEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenExponentialEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_exponentialEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenExponentialEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_exponentialEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenExponentialEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_exponentialEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenElasticEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_elasticEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenElasticEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_elasticEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenElasticEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_elasticEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBackEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_backEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBackEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_backEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBackEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_backEaseInOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBounceEaseOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_bounceEaseOut(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBounceEaseIn(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_bounceEaseIn(args[0]->NumberValue())));
// }
// static V8Return _OPtweenBounceEaseInOut(const V8Args& args) {
// 	V8Scope scope;	
// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPtween_bounceEaseInOut(args[0]->NumberValue())));
// }


// #endif