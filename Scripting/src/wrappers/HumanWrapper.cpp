#include "../../include/wrappers/HumanWrapper.h"

#ifdef OPIFEX_V8

#include "./Human/include/Input/Input.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/Texture.h"

static V8Return _OPrenderInit(const V8Args& args);
static V8Return _Clear(const V8Args& args);
static V8Return _Present(const V8Args& args);
static V8Return _CreateEffect(const V8Args& args);
static V8Return _CreateCamera(const V8Args& args);
static V8Return _DestroyCamera(const V8Args& args);
static V8Return _GetCameraView(const V8Args& args);
static V8Return _GetCameraProj(const V8Args& args);

static V8Return _BindMesh(const V8Args& args);
static V8Return _BindEffect(const V8Args& args);
static V8Return _RenderMesh(const V8Args& args);

static V8Return _TextureClearActive(const V8Args& args);
static V8Return _RenderParamMat4v(const V8Args& args);
static V8Return _RenderParamVec3(const V8Args& args);
static V8Return _RenderParamTexture(const V8Args& args);

static V8Return _InputUpdate(const V8Args& args);
static V8Return _Keyboard(const V8Args& args);
static V8Return _Mouse(const V8Args& args);




V8ObjectGlobal GetKeyboardMap() {
	V8ObjectGlobal keyboard = CreateObjectG(isolate);

	i8* keyNames[OPKEYBOARD_MAX] = {
			"BACKSPACE",
			"TAB",
			"ENTER",
			"PAUSE",
			"CAPSLOCK",
			"ESCAPE",
			"SPACE",
			"PAGEUP",
			"PAGEDOWN",
			"END",
			"HOME",
			"LEFT",
			"UP",
			"RIGHT",
			"DOWN",
			"INSERT",
			"DELETE",
			"0",
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9",
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
			"G",
			"H",
			"I",
			"J",
			"K",
			"L",
			"M",
			"N",
			"O",
			"P",
			"Q",
			"R",
			"S",
			"T",
			"U",
			"V",
			"W",
			"X",
			"Y",
			"Z",
			"LWIN",
			"RWIN",
			"NUMPAD0",
			"NUMPAD1",
			"NUMPAD2",
			"NUMPAD3",
			"NUMPAD4",
			"NUMPAD5",
			"NUMPAD6",
			"NUMPAD7",
			"NUMPAD8",
			"NUMPAD9",
			"MULTIPLY",
			"ADD",
			"SUBTRACT",
			"DECIMAL",
			"DIVIDE",
			"F1",
			"F2",
			"F3",
			"F4",
			"F5",
			"F6",
			"F7",
			"F8",
			"F9",
			"F10",
			"F11",
			"F12",
			"F13",
			"F14",
			"F15",
			"F16",
			"NUMLOCK",
			"SCROLL",
			"LSHIFT",
			"RSHIFT",
			"LCONTROL",
			"RCONTROL"
	};

	for (i32 i = 0; i < OPKEY_RCONTROL; i++) {
		i8* name = keyNames[i];
		SetValueG(isolate, keyboard, name, GetNumber(isolate, i));
	}

	return keyboard;
}


void HumanInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	SetObjectG(isolate, target, "KEYS", GetKeyboardMap());

	// OP
	SetFunctionG(isolate, target, "Clear", _Clear);
	SetFunctionG(isolate, target, "Present", _Present);

	// OP.render
	V8ObjectGlobal render = CreateObjectG(isolate);
	SetFunctionG(isolate, render, "Init", _OPrenderInit);
	SetFunctionG(isolate, render, "BindMesh", _BindMesh);
	SetFunctionG(isolate, render, "BindEffect", _BindEffect);
	SetFunctionG(isolate, render, "Mesh", _RenderMesh);
	SetFunctionG(isolate, render, "ParamMat4v", _RenderParamMat4v);
	SetFunctionG(isolate, render, "ParamVec3", _RenderParamVec3);
	SetFunctionG(isolate, render, "ParamTexture", _RenderParamTexture);
	SetFunctionG(isolate, render, "ClearActiveTextures", _TextureClearActive);
	SetObjectG(isolate, target, "render", render);

	// OP.camera
	V8ObjectGlobal camera = CreateObjectG(isolate);
	SetFunctionG(isolate, camera, "Create", _CreateCamera);
	SetFunctionG(isolate, camera, "Destroy", _DestroyCamera);
	SetFunctionG(isolate, camera, "View", _GetCameraView);
	SetFunctionG(isolate, camera, "Proj", _GetCameraProj);
	SetObjectG(isolate, target, "camera", camera);

	// OP.effect
	V8ObjectGlobal effect = CreateObjectG(isolate);
	SetFunctionG(isolate, effect, "Create", _CreateEffect);
	SetObjectG(isolate, target, "effect", effect);

	// OP.input
	V8ObjectGlobal input = CreateObjectG(isolate);
	SetFunctionG(isolate, input, "Update", _InputUpdate);
	SetFunctionG(isolate, input, "Keyboard", _Keyboard);
	SetFunctionG(isolate, input, "Mouse", _Mouse);
	SetObjectG(isolate, target, "input", input);

}


static V8Return _OPrenderInit(const V8Args& args) {
	V8Scope scope;

	OPrenderInit(1280, 720, false);

	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { GetBool(isolate, true) };
	Handle<Object> obj = CreateObject(isolate);
	cb->Call(obj, argc, argv);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Clear(const V8Args& args) {
	V8Scope scope;

	OPrenderClear(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Present(const V8Args& args) {
	V8Scope scope;

	OPrenderPresent();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _CreateEffect(const V8Args& args) {
	V8Scope scope;

	Local<Object> obj = args[0]->ToObject();
	Handle<Array> attributesArray = Handle<Array>::Cast(obj->Get(GetString(isolate, "Attributes")));
	i32 attributeCount = attributesArray->Length();
	OPshaderAttribute* attribs = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute)* attributeCount);
	for (i32 i = 0; i < attributeCount; i++) {
		Local<Object> first = attributesArray->Get(i)->ToObject();
		Local<Value> name = first->Get(GetString(isolate, "Id"));
		Local<Value> type = first->Get(GetString(isolate, "Type"));
		Local<Value> size = first->Get(GetString(isolate, "Size"));

		v8::String::Utf8Value utf8(name);
		const char* file = ToCString(utf8);
		i32 nameLength = strlen(file);
		attribs[i].Name = (char*)OPalloc(sizeof(char)* (nameLength + 1));
		char* nameTmp = (char*)OPalloc(sizeof(char)* (nameLength + 1));
		OPmemcpy((void*)nameTmp, (void*)file, sizeof(char)* nameLength);
		nameTmp[nameLength] = NULL;
		OPmemcpy((void*)attribs[i].Name, (void*)nameTmp, sizeof(char)* (nameLength + 1));

		if (!type.IsEmpty()){
			i32 typeNumber = type->Int32Value();
			if (typeNumber == 0) {
				attribs[i].Type = GL_FLOAT;
			}
		}

		attribs[i].Elements = size->Uint32Value();
	}

	OPshader* vert = (OPshader*)obj->Get(GetString(isolate, "Vertex"))->Int32Value();
	OPshader* frag = (OPshader*)obj->Get(GetString(isolate, "Fragment"))->Int32Value();
	const char *effectName = *v8::String::Utf8Value(obj->Get(GetString(isolate, "Name"))->ToString());

	OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		attributeCount,
		"Model Effect",
		32
		);

	OPfree(attribs);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)effect));
}

static V8Return _InputUpdate(const V8Args& args) {
	V8Scope scope;

	OPkeyboardUpdate();
	OPmouseUpdate();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Mouse(const V8Args& args) {
	V8Scope scope;

	V8Object obj = CreateObject(isolate);
	obj->Set(GetString(isolate, "X"), GetNumber(isolate, OPmousePositionX()));
	obj->Set(GetString(isolate, "Y"), GetNumber(isolate, OPmousePositionY()));
	obj->Set(GetString(isolate, "dX"), GetNumber(isolate, OPmousePositionMovedX()));
	obj->Set(GetString(isolate, "dY"), GetNumber(isolate, OPmousePositionMovedY()));
	obj->Set(GetString(isolate, "Wheel"), GetNumber(isolate, OPmouseWheel()));
	obj->Set(GetString(isolate, "dWheel"), GetNumber(isolate, OPmouseWheelMoved()));
	obj->Set(GetString(isolate, "LButton"), GetNumber(isolate, OPmouseIsDown(OPKEY_LBUTTON)));
	obj->Set(GetString(isolate, "RButton"), GetNumber(isolate, OPmouseIsDown(OPKEY_RBUTTON)));
	obj->Set(GetString(isolate, "LButtonRelease"), GetNumber(isolate, OPmouseWasPressed(OPKEY_LBUTTON)));
	obj->Set(GetString(isolate, "RButtonRelease"), GetNumber(isolate, OPmouseWasPressed(OPKEY_RBUTTON)));

	return SetReturn(args, &scope, obj);
}

static V8Return _Keyboard(const V8Args& args) {
	V8Scope scope;

	Handle<Array> arr = CreateArray(isolate);
	for (i32 i = 0; i < OPKEYBOARD_MAX_VALUE; i++) {
		arr->Set(i, GetNumber(isolate, OPkeyboardIsDown((OPkeyboardKey)i)));
	}

	return SetReturn(args, &scope, arr);
}

static V8Return _DestroyCamera(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	OPfree(camera);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _CreateCamera(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = OPcamProj(
		OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()),
		OPvec3Create(0, 0, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);
	OPrenderDepth(1);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)camera));
}

static V8Return _GetCameraView(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4* view = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPcamGetView((*camera), view);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)view));
}

static V8Return _GetCameraProj(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4* proj = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPcamGetProj((*camera), proj);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)proj));
}

static V8Return _BindMesh(const V8Args& args) {
	V8Scope scope;

	OPrenderBindMesh((OPmesh*)args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _BindEffect(const V8Args& args) {
	V8Scope scope;

	OPrenderBindEffect((OPeffect*)args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderMesh(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4 view, proj;

	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);

	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderMesh();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _TextureClearActive(const V8Args& args) {
	V8Scope scope;

	OPtextureClearActive();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderParamMat4v(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	ui32 len = strlen(p);

	OPchar* name = (i8*)OPalloc(sizeof(i8)* (len + 1));
	OPmemcpy(name, p, sizeof(i8)*len);
	name[len] = NULL;

	OPmat4* mat = (OPmat4*)args[1]->ToObject()->Get(GetString(isolate, "Id"))->Int32Value();
	OPrenderParamMat4v(name, 1, mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderParamVec3(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParamVec3(p, 1, (OPvec3*)args[1]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderParamTexture(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParami(p, OPtextureBind((OPtexture*)args[1]->Int32Value()));

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);

	return SetReturn(args, &scope, GetNull(isolate));
}

#endif