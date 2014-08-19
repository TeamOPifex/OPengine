#include "../../include/wrappers/HumanWrapper.h"

#ifdef OPIFEX_V8

#include "./Human/include/Input/Input.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/Texture.h"
#include "./Human/include/Rendering/Font/FontManager.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Human/include/Input/GamePadSystem.h"
#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Pipeline/include/Sprite3D.h"

static V8Return _OPrenderInit(const V8Args& args);
static V8Return _Clear(const V8Args& args);
static V8Return _Depth(const V8Args& args);
static V8Return _Present(const V8Args& args);
static V8Return _CreateEffect(const V8Args& args);
static V8Return _CreateCamera(const V8Args& args);
static V8Return _CameraSetPos(const V8Args& args);
static V8Return _CameraSetTarget(const V8Args& args);
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
static V8Return _GamePadLeftThumbX(const V8Args& args);
static V8Return _GamePadLeftThumbY(const V8Args& args);
static V8Return _GamePadLeftThumbIs(const V8Args& args);
static V8Return _GamePadLeftThumbNow(const V8Args& args);
static V8Return _GamePadLeftThumbWas(const V8Args& args);
static V8Return _GamePadRightThumbIs(const V8Args& args);
static V8Return _GamePadRightThumbNow(const V8Args& args);
static V8Return _GamePadRightThumbWas(const V8Args& args);
static V8Return _GamePadRightThumbX(const V8Args& args);
static V8Return _GamePadRightThumbY(const V8Args& args);
static V8Return _GamePadIsDown(const V8Args& args);
static V8Return _GamePadIsUp(const V8Args& args);
static V8Return _GamePadWasPressed(const V8Args& args);
static V8Return _GamePadWasReleased(const V8Args& args);
static V8Return _GamePadIsConnected(const V8Args& args);
static V8Return _GamePadSetDeadzone(const V8Args& args);

static V8Return _KeyboardIsDown(const V8Args& args);
static V8Return _KeyboardIsUp(const V8Args& args);
static V8Return _KeyboardWasReleased(const V8Args& args);
static V8Return _KeyboardWasPressed(const V8Args& args);


static V8Return _FontInit(const V8Args& args);
static V8Return _FontManagerCreate(const V8Args& args);
static V8Return _FontManagerBind(const V8Args& args);
static V8Return _FontManagerSetRGBA(const V8Args& args);
static V8Return _FontManagerSetAlign(const V8Args& args);
static V8Return _FontManagerAddText(const V8Args& args);
static V8Return _FontManagerBuild(const V8Args& args);
static V8Return _FontRenderText(const V8Args& args);
static V8Return _FontRenderTextMatrix(const V8Args& args);

static V8Return _Sprite3DCreate(const V8Args& args);
static V8Return _Sprite3DInit(const V8Args& args);
static V8Return _Sprite3DUpdate(const V8Args& args);
static V8Return _Sprite3DRender(const V8Args& args);


V8ObjectGlobal GetKeyboardMap() {
	V8ObjectGlobal keyboard = CreateObjectG(isolate);

	const OPchar* keyNames[OPKEYBOARD_MAX] = {
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
		const OPchar* name = keyNames[i];
		SetValueG(isolate, keyboard, name, GetNumber(isolate, i));
	}

	return keyboard;
}
V8Object GetKeyboardMapO() {
	V8Object keyboard = CreateObject(isolate);

	const OPchar* keyNames[OPKEYBOARD_MAX] = {
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
		const OPchar* name = keyNames[i];
		SetValue(isolate, keyboard, name, GetNumber(isolate, i));
	}

	return keyboard;
}

V8ObjectGlobal GetButtonMap() {
	V8ObjectGlobal buttons = CreateObjectG(isolate);

	const OPchar* keyNames[GamePadButton_Max] = {
		"DPAD_UP",
		"DPAD_DOWN",
		"DPAD_LEFT",
		"DPAD_RIGHT",
		"START",
		"BACK",
		"LEFT_THUMB",
		"RIGHT_THUMB",
		"LEFT_SHOULDER",
		"RIGHT_SHOULDER",
		"A",
		"B",
		"X",
		"Y"
	};

	for (i32 i = 0; i < GamePadButton_Max; i++) {
		const OPchar* name = keyNames[i];
		SetValueG(isolate, buttons, name, GetNumber(isolate, i));
	}

	return buttons;
}

V8Object GetButtonMapO() {
	V8Object buttons = CreateObject(isolate);

	const OPchar* keyNames[GamePadButton_Max] = {
		"DPAD_UP",
		"DPAD_DOWN",
		"DPAD_LEFT",
		"DPAD_RIGHT",
		"START",
		"BACK",
		"LEFT_THUMB",
		"RIGHT_THUMB",
		"LEFT_SHOULDER",
		"RIGHT_SHOULDER",
		"A",
		"B",
		"X",
		"Y"
	};

	for (i32 i = 0; i < GamePadButton_Max; i++) {
		const OPchar* name = keyNames[i];
		SetValue(isolate, buttons, name, GetNumber(isolate, i));
	}

	return buttons;
}

V8ObjectGlobal GetFacingMap() {
	V8ObjectGlobal facing = CreateObjectG(isolate);

	SetValueG(isolate, facing, "LEFT", GetNumber(isolate, 0));
	SetValueG(isolate, facing, "UP", GetNumber(isolate, 1));
	SetValueG(isolate, facing, "RIGHT", GetNumber(isolate, 2));
	SetValueG(isolate, facing, "DOWN", GetNumber(isolate, 3));

	return facing;
}

V8Object GetFacingMapO() {
	V8Object facing = CreateObject(isolate);

	SetValue(isolate, facing, "LEFT", GetNumber(isolate, 0));
	SetValue(isolate, facing, "UP", GetNumber(isolate, 1));
	SetValue(isolate, facing, "RIGHT", GetNumber(isolate, 2));
	SetValue(isolate, facing, "DOWN", GetNumber(isolate, 3));

	return facing;
}

void HumanInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	SetObjectG(isolate, target, "KEYS", GetKeyboardMap());
	SetObjectG(isolate, target, "BUTTONS", GetButtonMap());
	SetObjectG(isolate, target, "FACING", GetFacingMap());

	// OP
	SetFunctionG(isolate, target, "Clear", _Clear);
	SetFunctionG(isolate, target, "Depth", _Depth);
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
	SetFunctionG(isolate, camera, "SetPos", _CameraSetPos);
	SetFunctionG(isolate, camera, "SetTarget", _CameraSetTarget);
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
	SetFunctionG(isolate, input, "KeyIsDown", _KeyboardIsDown);
	SetFunctionG(isolate, input, "KeyIsUp", _KeyboardIsUp);
	SetFunctionG(isolate, input, "KeyWasReleased", _KeyboardWasReleased);
	SetFunctionG(isolate, input, "KeyWasPressed", _KeyboardWasPressed);
	SetFunctionG(isolate, input, "GamePadLeftThumbX", _GamePadLeftThumbX);
	SetFunctionG(isolate, input, "GamePadLeftThumbY", _GamePadLeftThumbY);
	SetFunctionG(isolate, input, "GamePadLeftThumbIs", _GamePadLeftThumbIs);
	SetFunctionG(isolate, input, "GamePadLeftThumbNow", _GamePadLeftThumbNow);
	SetFunctionG(isolate, input, "GamePadLeftThumbWas", _GamePadLeftThumbWas);
	SetFunctionG(isolate, input, "GamePadRightThumbIs", _GamePadRightThumbIs);
	SetFunctionG(isolate, input, "GamePadRightThumbNow", _GamePadRightThumbNow);
	SetFunctionG(isolate, input, "GamePadRightThumbWas", _GamePadRightThumbWas);
	SetFunctionG(isolate, input, "GamePadRightThumbX", _GamePadRightThumbX);
	SetFunctionG(isolate, input, "GamePadRightThumbY", _GamePadRightThumbY);
	SetFunctionG(isolate, input, "GamePadIsDown", _GamePadIsDown);
	SetFunctionG(isolate, input, "GamePadIsUp", _GamePadIsUp);
	SetFunctionG(isolate, input, "GamePadWasPressed", _GamePadWasPressed);
	SetFunctionG(isolate, input, "GamePadWasReleased", _GamePadWasReleased);
	SetFunctionG(isolate, input, "GamePadIsConnected", _GamePadIsConnected);
	SetFunctionG(isolate, input, "GamePadSetDeadzone", _GamePadSetDeadzone);
	SetObjectG(isolate, target, "input", input);

	// OP.font
	V8ObjectGlobal font = CreateObjectG(isolate);
	SetFunctionG(isolate, font, "Init", _FontInit);
	SetFunctionG(isolate, font, "CreateManager", _FontManagerCreate);
	SetFunctionG(isolate, font, "RenderText", _FontRenderText);
	SetFunctionG(isolate, font, "RenderTextMat4", _FontRenderTextMatrix);
	SetObjectG(isolate, target, "font", font);

	// OP.sprite3D
	V8ObjectGlobal sprite3D = CreateObjectG(isolate);
	SetFunctionG(isolate, sprite3D, "Init", _Sprite3DInit);
	SetFunctionG(isolate, sprite3D, "Create", _Sprite3DCreate);
	SetFunctionG(isolate, sprite3D, "Update", _Sprite3DUpdate);
	SetFunctionG(isolate, sprite3D, "Render", _Sprite3DRender);
	SetObjectG(isolate, target, "sprite3D", sprite3D);

}

void HumanInitializeMethodsO(V8isolate* isolate, V8Object target) {

	SetObject(isolate, target, "KEYS", GetKeyboardMapO());
	SetObject(isolate, target, "BUTTONS", GetButtonMapO());
	SetObject(isolate, target, "FACING", GetFacingMapO());

	// OP
	SetFunction(isolate, target, "Clear", _Clear);
	SetFunction(isolate, target, "Depth", _Depth);
	SetFunction(isolate, target, "Present", _Present);

	// OP.render
	V8Object render = CreateObject(isolate);
	SetFunction(isolate, render, "Init", _OPrenderInit);
	SetFunction(isolate, render, "BindMesh", _BindMesh);
	SetFunction(isolate, render, "BindEffect", _BindEffect);
	SetFunction(isolate, render, "Mesh", _RenderMesh);
	SetFunction(isolate, render, "ParamMat4v", _RenderParamMat4v);
	SetFunction(isolate, render, "ParamVec3", _RenderParamVec3);
	SetFunction(isolate, render, "ParamTexture", _RenderParamTexture);
	SetFunction(isolate, render, "ClearActiveTextures", _TextureClearActive);
	SetObject(isolate, target, "render", render);

	// OP.camera
	V8Object camera = CreateObject(isolate);
	SetFunction(isolate, camera, "Create", _CreateCamera);
	SetFunction(isolate, camera, "Destroy", _DestroyCamera);
	SetFunction(isolate, camera, "View", _GetCameraView);
	SetFunction(isolate, camera, "Proj", _GetCameraProj);
	SetFunction(isolate, camera, "SetPos", _CameraSetPos);
	SetFunction(isolate, camera, "SetTarget", _CameraSetTarget);
	SetObject(isolate, target, "camera", camera);

	// OP.effect
	V8Object effect = CreateObject(isolate);
	SetFunction(isolate, effect, "Create", _CreateEffect);
	SetObject(isolate, target, "effect", effect);

	// OP.input
	V8Object input = CreateObject(isolate);
	SetFunction(isolate, input, "Update", _InputUpdate);
	SetFunction(isolate, input, "Keyboard", _Keyboard);
	SetFunction(isolate, input, "Mouse", _Mouse);
	SetFunction(isolate, input, "KeyIsDown", _KeyboardIsDown);
	SetFunction(isolate, input, "KeyIsUp", _KeyboardIsUp);
	SetFunction(isolate, input, "KeyWasReleased", _KeyboardWasReleased);
	SetFunction(isolate, input, "KeyWasPressed", _KeyboardWasPressed);
	SetFunction(isolate, input, "GamePadLeftThumbX", _GamePadLeftThumbX);
	SetFunction(isolate, input, "GamePadLeftThumbY", _GamePadLeftThumbY);
	SetFunction(isolate, input, "GamePadLeftThumbIs", _GamePadLeftThumbIs);
	SetFunction(isolate, input, "GamePadLeftThumbNow", _GamePadLeftThumbNow);
	SetFunction(isolate, input, "GamePadLeftThumbWas", _GamePadLeftThumbWas);
	SetFunction(isolate, input, "GamePadRightThumbIs", _GamePadRightThumbIs);
	SetFunction(isolate, input, "GamePadRightThumbNow", _GamePadRightThumbNow);
	SetFunction(isolate, input, "GamePadRightThumbWas", _GamePadRightThumbWas);
	SetFunction(isolate, input, "GamePadRightThumbX", _GamePadRightThumbX);
	SetFunction(isolate, input, "GamePadRightThumbY", _GamePadRightThumbY);
	SetFunction(isolate, input, "GamePadIsDown", _GamePadIsDown);
	SetFunction(isolate, input, "GamePadIsUp", _GamePadIsUp);
	SetFunction(isolate, input, "GamePadWasPressed", _GamePadWasPressed);
	SetFunction(isolate, input, "GamePadWasReleased", _GamePadWasReleased);
	SetFunction(isolate, input, "GamePadIsConnected", _GamePadIsConnected);
	SetFunction(isolate, input, "GamePadSetDeadzone", _GamePadSetDeadzone);
	SetObject(isolate, target, "input", input);

	// OP.font
	V8Object font = CreateObject(isolate);
	SetFunction(isolate, font, "Init", _FontInit);
	SetFunction(isolate, font, "CreateManager", _FontManagerCreate);
	SetFunction(isolate, font, "RenderText", _FontRenderText);
	SetFunction(isolate, font, "RenderTextMat4", _FontRenderTextMatrix);
	SetObject(isolate, target, "font", font);

	// OP.sprite3D
	V8Object sprite3D = CreateObject(isolate);
	SetFunction(isolate, sprite3D, "Init", _Sprite3DInit);
	SetFunction(isolate, sprite3D, "Create", _Sprite3DCreate);
	SetFunction(isolate, sprite3D, "Update", _Sprite3DUpdate);
	SetFunction(isolate, sprite3D, "Render", _Sprite3DRender);
	SetObject(isolate, target, "sprite3D", sprite3D);

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

static V8Return _Depth(const V8Args& args) {
	V8Scope scope;

	OPrenderDepth(args[0]->Int32Value());

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
		obj->Get(GetString(isolate, "Stride"))->Int32Value()
		);

	OPfree(attribs);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)effect));
}

static V8Return _InputUpdate(const V8Args& args) {
	V8Scope scope;

	OPkeyboardUpdate();
	OPmouseUpdate();
	OPgamePadSystemUpdate();

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

static V8Return _KeyboardIsDown(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardIsDown((OPkeyboardKey)args[0]->Int32Value());
	return SetReturn(args, &scope, GetBool(isolate, result));
}

static V8Return _KeyboardIsUp(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardIsUp((OPkeyboardKey)args[0]->Int32Value());
	return SetReturn(args, &scope, GetBool(isolate, result));
}

static V8Return _KeyboardWasReleased(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardWasReleased((OPkeyboardKey)args[0]->Int32Value());
	return SetReturn(args, &scope, GetBool(isolate, result));
}

static V8Return _KeyboardWasPressed(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardWasPressed((OPkeyboardKey)args[0]->Int32Value());
	return SetReturn(args, &scope, GetBool(isolate, result));
}

static V8Return _GamePadLeftThumbX(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPgamePadLeftThumbX(OPgamePad((GamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadLeftThumbY(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPgamePadLeftThumbY(OPgamePad((GamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadLeftThumbIs(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbIsLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbIsUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbIsRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbIsDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadLeftThumbNow(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbNowLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbNowUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbNowRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbNowDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadLeftThumbWas(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbWasLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbWasUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbWasRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadLeftThumbWasDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadRightThumbIs(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbIsLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbIsUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbIsRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbIsDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadRightThumbNow(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbNowLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbNowUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbNowRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbNowDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadRightThumbWas(const V8Args& args) {
	V8Scope scope;
	OPgamePadController* controller = OPgamePad((GamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbWasLeft(controller)));
		break;
	case 1:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbWasUp(controller)));
		break;
	case 2:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbWasRight(controller)));
		break;
	case 3:
		return SetReturn(args, &scope, GetBool(isolate, OPgamePadRightThumbWasDown(controller)));
		break;
	default:
		return SetReturn(args, &scope, GetBool(isolate, false));
		break;
	}
	return SetReturn(args, &scope, GetBool(isolate, false));
}
static V8Return _GamePadRightThumbX(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPgamePadRightThumbX(OPgamePad((GamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadRightThumbY(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPgamePadRightThumbY(OPgamePad((GamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadIsDown(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetBool(isolate, OPgamePadIsDown(OPgamePad((GamePadIndex)args[0]->Int32Value()), (GamePadButton)args[1]->Int32Value())));
}
static V8Return _GamePadIsUp(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetBool(isolate, OPgamePadIsUp(OPgamePad((GamePadIndex)args[0]->Int32Value()), (GamePadButton)args[1]->Int32Value())));
}
static V8Return _GamePadWasPressed(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetBool(isolate, OPgamePadWasPressed(OPgamePad((GamePadIndex)args[0]->Int32Value()), (GamePadButton)args[1]->Int32Value())));

}
static V8Return _GamePadWasReleased(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetBool(isolate, OPgamePadWasReleased(OPgamePad((GamePadIndex)args[0]->Int32Value()), (GamePadButton)args[1]->Int32Value())));

}
static V8Return _GamePadIsConnected(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetBool(isolate, OPgamePadIsConnected(OPgamePad((GamePadIndex)args[0]->Int32Value()))));

}
static V8Return _GamePadSetDeadzone(const V8Args& args) {
	V8Scope scope;
	OPgamePadSetDeadzone(OPgamePad((GamePadIndex)args[0]->Int32Value()), args[1]->NumberValue());
	return SetReturn(args, &scope, GetNull(isolate));
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
		OPvec3Create(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue()),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	V8Object obj = CreateTypedObject(isolate, camera, OPscript_CAMERA);
	SetFunction(isolate, obj, "SetPos", _CameraSetPos);
	SetFunction(isolate, obj, "SetTarget", _CameraSetTarget);
	
	return SetReturn(args, &scope, obj);
}

static V8Return _CameraSetPos(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (IsObject(args, isolate, 1 - inScope, OPscript_VEC3)) {
		OPvec3* pos = (OPvec3*)GetArgPointer(args, isolate, 1 - inScope);
		OPcamSetPosition(camera, (*pos));
	}
	else if(args.Length() > 2) {
		OPvec3 position = OPvec3Create(
			args[1 - inScope]->NumberValue(),
			args[2 - inScope]->NumberValue(),
			args[3 - inScope]->NumberValue());
		OPcamSetPosition(camera, position);
	}

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _CameraSetTarget(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (IsObject(args, isolate, 1 - inScope, OPscript_VEC3)) {
		OPvec3* target = (OPvec3*)GetArgPointer(args, isolate, 1 - inScope);
		OPcamSetTarget(camera, (*target));
	}
	else {
		OPvec3 target = OPvec3Create(
			args[1 - inScope]->NumberValue(), 
			args[2 - inScope]->NumberValue(), 
			args[3 - inScope]->NumberValue());
		OPcamSetTarget(camera, target);
	}

	return SetReturn(args, &scope, GetNumber(isolate, (i32)camera));
}

static V8Return _GetCameraView(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	OPmat4* view = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPcamGetView((*camera), view);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)view));
}

static V8Return _GetCameraProj(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

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

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

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

static V8Return _FontInit(const V8Args& args) {
	V8Scope scope;

	OPsystemsLoadFontEffect();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerCreate(const V8Args& args) {
	V8Scope scope;

	OPfont* font = (OPfont*)args[0]->Int32Value();
	OPfontManager* manager = OPfontManagerCreate(font);

	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", GetNumber(isolate, (i32)manager));
	SetFunction(isolate, obj, "Bind", _FontManagerBind);
	SetFunction(isolate, obj, "SetRGBA", _FontManagerSetRGBA);
	SetFunction(isolate, obj, "SetAlign", _FontManagerSetAlign);
	SetFunction(isolate, obj, "AddText", _FontManagerAddText);
	SetFunction(isolate, obj, "Build", _FontManagerBuild);

	return SetReturn(args, &scope, obj);
}

static V8Return _FontManagerBind(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)GetPointer(args, isolate, &inScope, 1);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	OPfontManagerBind(manager);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerSetRGBA(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)GetPointer(args, isolate, &inScope, 5);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	OPfontManagerSetRGBA(manager, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue(), args[4 - inScope]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerSetAlign(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));

	OPfontManagerSetAlign(manager, (OPfontAlign)args[1 - inScope]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerAddText(const V8Args& args) {
	V8Scope scope;

	OPfontManagerAddText(ToCString(String::Utf8Value(args[0]->ToString())));

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerBuild(const V8Args& args) {
	V8Scope scope;

	OPfontManagerBuild();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontRenderText(const V8Args& args) {
	V8Scope scope;

	OPrenderTextXY(ToCString(String::Utf8Value(args[0]->ToString())), args[1]->NumberValue(), args[2]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontRenderTextMatrix(const V8Args& args) {
	V8Scope scope;

	OPrenderTextMat4(ToCString(String::Utf8Value(args[0]->ToString())), (OPmat4*)args[1]->ToObject()->Get(GetString(isolate, "Id"))->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Sprite3DInit(const V8Args& args) {
	V8Scope scope;

	OPsprite3DInit(NULL);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Sprite3DCreate(const V8Args& args) {
	V8Scope scope;

	Handle<Array> spritesArray = Handle<Array>::Cast(args[0]);

	OPsprite** sprites = (OPsprite**)OPalloc(sizeof(OPsprite*)* spritesArray->Length());
	for (i32 i = 0; i < spritesArray->Length(); i++){
		sprites[i] = (OPsprite*)spritesArray->Get(i)->Int32Value();
	}

	OPsprite3D* sprite = OPsprite3DCreate(sprites, NULL);

	OPfree(sprites);
	
	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", GetNumber(isolate, (i32)sprite));
	SetFunction(isolate, obj, "Update", _Sprite3DUpdate);
	SetFunction(isolate, obj, "Render", _Sprite3DRender);
	SetValue(isolate, obj, "Position", GetNumber(isolate, (i32)&sprite->Position));

	return SetReturn(args, &scope, obj);
}

static V8Return _Sprite3DUpdate(const V8Args& args) {
	V8Scope scope;

	i32 result;
	OPsprite3D* sprite = (OPsprite3D*)GetPointer(args, isolate, &result, 2);
	OPsprite3DUpdate(sprite, args[1 - result]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _Sprite3DRender(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPsprite3D* sprite = (OPsprite3D*)GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) SetReturn(args, &scope, GetNull(isolate));
	OPcam* camera = (OPcam*)GetArgPointer(args, isolate, 1 - inScope);
	OPsprite3DRender(sprite, camera);

	return SetReturn(args, &scope, GetNull(isolate));
}

#endif