#include "../../include/wrappers/HumanWrapper.h"

#ifdef OPIFEX_V8

#include "./Human/include/Input/Input.h"
#include "./Human/include/Rendering/FrameBuffer.h"
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
#include "./Human/include/Audio/Audio.h"
#include "./Human/include/Audio/AudioPlayer.h"
#include "./Human/include/Input/Myo.h"

// Render
static V8Return _OP_render_Init(const V8Args& args);
static V8Return _OP_render_Clear(const V8Args& args);
static V8Return _OP_render_Blend(const V8Args& args);
static V8Return _OP_render_BlendAlpha(const V8Args& args);
static V8Return _OP_render_Depth(const V8Args& args);
static V8Return _OP_render_Cull(const V8Args& args);
static V8Return _OP_render_CullMode(const V8Args& args);
static V8Return _OP_render_Present(const V8Args& args);
static V8Return _OP_render_CreateMesh(const V8Args& args);
static V8Return _OP_render_BuildMesh(const V8Args& args);
static V8Return _OP_render_FullScreen(const V8Args& args);
static V8Return _OP_render_ScreenSize(const V8Args& args);

static V8Return _CreateProj(const V8Args& args);
static V8Return _CreateOrtho(const V8Args& args);
static V8Return _CameraSetPos(const V8Args& args);
static V8Return _CameraSetTarget(const V8Args& args);
static V8Return _DestroyCamera(const V8Args& args);
static V8Return _GetCameraView(const V8Args& args);
static V8Return _GetCameraProj(const V8Args& args);

static V8Return _OP_effect_Create(const V8Args& args);

static V8Return _OP_render_BindMesh(const V8Args& args);
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

static V8Return _OP_myo_Connect(const V8Args& args);
static V8Return _OP_myo_Update(const V8Args& args);
static V8Return _OP_myo_Roll(const V8Args& args);
static V8Return _OP_myo_Pitch(const V8Args& args);
static V8Return _OP_myo_Yaw(const V8Args& args);
static V8Return _OP_myo_RollChange(const V8Args& args);
static V8Return _OP_myo_PitchChange(const V8Args& args);
static V8Return _OP_myo_YawChange(const V8Args& args);
static V8Return _OP_myo_SetVibration(const V8Args& args);
static V8Return _OP_myo_PoseNow(const V8Args& args);
static V8Return _OP_myo_PoseWas(const V8Args& args);
static V8Return _OP_myo_PoseIs(const V8Args& args);

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

static V8Return _AudioInit(const V8Args& args);
static V8Return _AudioCreate(const V8Args& args);
static V8Return _AudioBind(const V8Args& args);
static V8Return _AudioPlay(const V8Args& args);
static V8Return _AudioVolume(const V8Args& args);

static V8Return _FrameBufferBind(const V8Args& args);
static V8Return _FrameBufferUnbind(const V8Args& args);
static V8Return _FrameBufferCreate(const V8Args& args);
static V8Return _FrameBufferCreateDepth(const V8Args& args);

OPchar* keyNames[OPKEYBOARD_MAX] = {
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

OPchar* gamePadNames[GamePadButton_Max] = {
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

V8ObjectGlobal GetKeyboardMap() {
	V8ObjectGlobal keyboard = CreateObjectG(isolate);

	for (OPint i = 0; i < OPKEY_RCONTROL; i++) {
		OPchar* name = keyNames[i];
		SetValueG(isolate, keyboard, name, GetNumber(isolate, i));
	}

	return keyboard;
}

V8Object GetKeyboardMapO() {
	V8Object keyboard = CreateObject(isolate);

	for (OPint i = 0; i < OPKEY_RCONTROL; i++) {
		OPchar* name = keyNames[i];
		SetValue(isolate, keyboard, name, GetNumber(isolate, i));
	}

	return keyboard;
}

V8ObjectGlobal GetButtonMap() {
	V8ObjectGlobal buttons = CreateObjectG(isolate);


	for (OPint i = 0; i < GamePadButton_Max; i++) {
		OPchar* name = gamePadNames[i];
		SetValueG(isolate, buttons, name, GetNumber(isolate, i));
	}

	return buttons;
}

V8Object GetButtonMapO() {
	V8Object buttons = CreateObject(isolate);

	for (OPint i = 0; i < GamePadButton_Max; i++) {
		OPchar* name = gamePadNames[i];
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

// void _initializeMethods(V8isolate* isolate, void* target, void(*setObject)(V8isolate*, void*, const OPchar*, void*), void(*setFunction)(V8isolate*, void*, const OPchar*, void*)) {

// 	setObject(isolate, target, "KEYS", GetKeyboardMap());
// }

void HumanInitializeMethodsO(V8isolate* isolate, V8Object target) {


	// OP
	SetObject(isolate, target, "KEYS", GetKeyboardMapO());
	SetObject(isolate, target, "BUTTONS", GetButtonMapO());
	SetObject(isolate, target, "FACING", GetFacingMapO());

	// OP.render
	V8Object render = CreateObject(isolate);
	SetFunction(isolate, render, "Clear", _OP_render_Clear);
	SetFunction(isolate, render, "Blend", _OP_render_Blend);
	SetFunction(isolate, render, "BlendAlpha", _OP_render_BlendAlpha);
	SetFunction(isolate, render, "Depth", _OP_render_Depth);
	SetFunction(isolate, render, "Cull", _OP_render_Cull);
	SetFunction(isolate, render, "CullMode", _OP_render_CullMode);
	SetFunction(isolate, render, "Present", _OP_render_Present);
	SetFunction(isolate, render, "Init", _OP_render_Init);
	SetFunction(isolate, render, "CreateMesh", _OP_render_CreateMesh);
	SetFunction(isolate, render, "BuildMesh", _OP_render_BuildMesh);
	SetFunction(isolate, render, "BindMesh", _OP_render_BindMesh);
	SetFunction(isolate, render, "BindEffect", _BindEffect);
	SetFunction(isolate, render, "Mesh", _RenderMesh);
	SetFunction(isolate, render, "ParamMat4v", _RenderParamMat4v);
	SetFunction(isolate, render, "ParamVec3", _RenderParamVec3);
	SetFunction(isolate, render, "ParamTexture", _RenderParamTexture);
	SetFunction(isolate, render, "ClearActiveTextures", _TextureClearActive);
	SetFunction(isolate, render, "FullScreen", _OP_render_FullScreen);
	SetFunction(isolate, render, "ScreenSize", _OP_render_ScreenSize);
	SetObject(isolate, target, "render", render);

	// OP.camera
	V8Object camera = CreateObject(isolate);
	SetFunction(isolate, camera, "CreateProj", _CreateProj);
	SetFunction(isolate, camera, "CreateOrtho", _CreateOrtho);
	SetFunction(isolate, camera, "Destroy", _DestroyCamera);
	SetFunction(isolate, camera, "View", _GetCameraView);
	SetFunction(isolate, camera, "Proj", _GetCameraProj);
	SetFunction(isolate, camera, "SetPos", _CameraSetPos);
	SetFunction(isolate, camera, "SetTarget", _CameraSetTarget);
	SetObject(isolate, target, "camera", camera);

	// OP.effect
	V8Object effect = CreateObject(isolate);
	SetFunction(isolate, effect, "Create", _OP_effect_Create);
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

	// OP.myo

	V8Object myo = CreateObject(isolate);
	SetFunction(isolate, myo, "Connect", _OP_myo_Connect);
	SetFunction(isolate, myo, "Update", _OP_myo_Update);
	SetFunction(isolate, myo, "Roll", _OP_myo_Roll);
	SetFunction(isolate, myo, "Pitch", _OP_myo_Pitch);
	SetFunction(isolate, myo, "Yaw", _OP_myo_Yaw);
	SetFunction(isolate, myo, "RollChange", _OP_myo_RollChange);
	SetFunction(isolate, myo, "PitchChange", _OP_myo_PitchChange);
	SetFunction(isolate, myo, "YawChange", _OP_myo_YawChange);
	SetFunction(isolate, myo, "SetVibration", _OP_myo_SetVibration);
	SetFunction(isolate, myo, "PoseNow", _OP_myo_PoseNow);
	SetFunction(isolate, myo, "PoseWas", _OP_myo_PoseWas);
	SetFunction(isolate, myo, "PoseIs", _OP_myo_PoseIs);
	SetObject(isolate, target, "myo", myo);

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


	// OP.audio
	V8Object audio = CreateObject(isolate);
	SetFunction(isolate, audio, "Init", _AudioInit);
	SetFunction(isolate, audio, "Create", _AudioCreate);
	SetFunction(isolate, audio, "Bind", _AudioBind);
	SetFunction(isolate, audio, "Play", _AudioPlay);
	SetFunction(isolate, audio, "Volume", _AudioVolume);
	SetObject(isolate, target, "audio", audio);


	// OP.frameBuffer	
	V8Object frameBuffer = CreateObject(isolate);
	SetFunction(isolate, frameBuffer, "Create", _FrameBufferCreate);
	SetFunction(isolate, frameBuffer, "CreateDepth", _FrameBufferCreateDepth);
	SetFunction(isolate, frameBuffer, "Bind", _FrameBufferBind);
	SetFunction(isolate, frameBuffer, "Unbind", _FrameBufferUnbind);
	SetObject(isolate, target, "frameBuffer", frameBuffer);
}



static V8Return _OP_render_Init(const V8Args& args) {
	V8Scope scope;

	OPrenderInit();

	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { GetBool(isolate, true) };
	Handle<Object> obj = CreateObject(isolate);
	cb->Call(obj, argc, argv);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_Clear(const V8Args& args) {
	V8Scope scope;

	OPrenderClear(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_Blend(const V8Args& args) {
	V8Scope scope;

	OPrenderBlend(args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_BlendAlpha(const V8Args& args) {
	V8Scope scope;

	OPrenderBlendAlpha();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_Depth(const V8Args& args) {
	V8Scope scope;

	OPrenderDepth(args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_Cull(const V8Args& args) {
	V8Scope scope;

	OPrenderCull(args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_CullMode(const V8Args& args) {
	V8Scope scope;

	OPrenderCullMode(args[0]->Int32Value());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_Present(const V8Args& args) {
	V8Scope scope;

	OPrenderPresent();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_CreateMesh(const V8Args& args) {
	V8Scope scope;

	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	*mesh = OPrenderCreateMesh();

	//V8Object obj = CreateTypedObject(isolate, mesh, OPscript_MESH);
	
	return SetReturn(args, &scope, GetNumber(isolate, (OPint)mesh));
}

static V8Return _OP_render_BindMesh(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	//OPmesh* mesh = (OPmesh*)GetFirstPointer(args, isolate, &inScope, OPscript_MESH);
	OPmesh* mesh = (OPmesh*)args[0]->IntegerValue();

	OPrenderBindMesh(mesh);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_BuildMesh(const V8Args& args) {
	V8Scope scope;

	i32 vertexElementCount = args[0]->Int32Value();

	Local<Object> obj = args[1]->ToObject();
	Handle<Array> vertDataArray = Handle<Array>::Cast(obj);
	i32 vertDataCount = vertDataArray->Length();

	obj = args[2]->ToObject();
	Handle<Array> indexDataArray = Handle<Array>::Cast(obj);
	i32 indexDataCount = indexDataArray->Length();

	OPfloat* verts = (OPfloat*)OPalloc(sizeof(OPfloat) * vertDataCount * vertexElementCount);
	ui16* indices = (ui16*)OPalloc(sizeof(ui16) * indexDataCount);

	for(i32 i = 0; i < vertDataCount; i++) {
		Handle<Array> vert = Handle<Array>::Cast(vertDataArray->Get(i));
		for(i32 j = 0; j < vertexElementCount; j++) {
			verts[i * vertexElementCount + j] = vert->Get(j)->NumberValue();
		}
	}

	for(i32 i = 0; i < indexDataCount; i++) {
		indices[i] = indexDataArray->Get(i)->Int32Value();
	}

	OPrenderBuildMesh(
		sizeof(OPfloat) * vertexElementCount,
		sizeof(ui16),
		vertDataCount,
		indexDataCount,
		verts,
		indices);

	OPfree(verts);
	OPfree(indices);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_FullScreen(const V8Args& args) {
	V8Scope scope;

	OPrenderFullscreen = args[0]->Int32Value();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_render_ScreenSize(const V8Args& args) {
	V8Scope scope;

	OPscreenWidth = args[0]->Int32Value();
	OPscreenHeight = args[1]->Int32Value();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _OP_effect_Create(const V8Args& args) {
	V8Scope scope;

	Local<Object> obj = args[0]->ToObject();
	Handle<Array> attributesArray = Handle<Array>::Cast(obj->Get(GetString(isolate, "Attributes")));
	i32 attributeCount = attributesArray->Length();

	OPlog("Attributes found: %d", attributeCount);

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
		OPchar* nameTmp = (char*)OPalloc(sizeof(char)* (nameLength + 1));
		strcpy(nameTmp, file);
		OPmemcpy((void*)attribs[i].Name, (void*)nameTmp, sizeof(OPchar)* (nameLength + 1));

		if (!type.IsEmpty()){
			i32 typeNumber = type->Int32Value();
			if (typeNumber == 0) {
				attribs[i].Type = GL_FLOAT;
			}
		}

		attribs[i].Elements = size->Uint32Value();
	}

	OPshader* vert = (OPshader*)obj->Get(GetString(isolate, "Vertex"))->IntegerValue();
	OPshader* frag = (OPshader*)obj->Get(GetString(isolate, "Fragment"))->IntegerValue();
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

	return SetReturn(args, &scope, GetNumber(isolate, (OPint)effect));
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


static V8Return _OP_myo_Connect(const V8Args& args) {
	V8Scope scope;
	OPmyoConnect();
	return SetReturn(args, &scope, GetBool(isolate, true));
}

static V8Return _OP_myo_Update(const V8Args& args) {
	V8Scope scope;
	OPmyoUpdate(OPgetTime());
	return SetReturn(args, &scope, GetNumberF32(isolate, true));
}

static V8Return _OP_myo_Roll(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoRoll()));
}
static V8Return _OP_myo_Pitch(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoPitch()));
}
static V8Return _OP_myo_Yaw(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoYaw()));
}
static V8Return _OP_myo_RollChange(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoRollChange()));
}
static V8Return _OP_myo_PitchChange(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoPitchChange()));
}
static V8Return _OP_myo_YawChange(const V8Args& args) {
	V8Scope scope;
	return SetReturn(args, &scope, GetNumberF32(isolate, OPmyoYawChange()));
}
static V8Return _OP_myo_SetVibration(const V8Args& args) {
	V8Scope scope;
	OPmyoVibrations v = (OPmyoVibrations)(args[0]->IntegerValue());
	OPmyoSetVibration(v);
	return SetReturn(args, &scope, GetBool(isolate, true));
}
static V8Return _OP_myo_PoseNow(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return SetReturn(args, &scope, GetNumber(isolate, OPmyoPoseNow(p)));
}
static V8Return _OP_myo_PoseWas(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return SetReturn(args, &scope, GetNumber(isolate, OPmyoPoseWas(p)));
}
static V8Return _OP_myo_PoseIs(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return SetReturn(args, &scope, GetNumber(isolate, OPmyoPoseIs(p)));
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

static V8Return _CreateProj(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = OPcamProj(
		OPvec3create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()),
		OPvec3create(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue()),
		OPvec3create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPlogVec3("Camera Pos", &camera->_pos);
	OPlogVec3("Camera Target", &camera->_targ);
	OPlog("Aspect: %f", OPrenderWidth / (f32)OPrenderHeight);

	V8Object obj = CreateTypedObject(isolate, camera, OPscript_CAMERA);
	SetFunction(isolate, obj, "SetPos", _CameraSetPos);
	SetFunction(isolate, obj, "SetTarget", _CameraSetTarget);
	SetFunction(isolate, obj, "View", _GetCameraView);
	SetFunction(isolate, obj, "Proj", _GetCameraProj);
	//SetValue(isolate, obj, "Position", (OPint)&camera->_pos);
	
	return SetReturn(args, &scope, obj);
}

static V8Return _CreateOrtho(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = OPcamOrtho(
		OPvec3create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()),
		OPvec3create(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue()),
		OPvec3create(0, 1, 0),
		0.1f,
		1000.0f,
		args[6]->NumberValue(),
		args[7]->NumberValue(),
		args[8]->NumberValue(),
		args[9]->NumberValue()
		);

	V8Object obj = CreateTypedObject(isolate, camera, OPscript_CAMERA);
	SetFunction(isolate, obj, "SetPos", _CameraSetPos);
	SetFunction(isolate, obj, "SetTarget", _CameraSetTarget);
	SetFunction(isolate, obj, "View", _GetCameraView);
	SetFunction(isolate, obj, "Proj", _GetCameraProj);
	
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
		OPvec3 position = OPvec3create(
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
		OPvec3 target = OPvec3create(
			args[1 - inScope]->NumberValue(), 
			args[2 - inScope]->NumberValue(), 
			args[3 - inScope]->NumberValue());
		OPcamSetTarget(camera, target);
	}

	return SetReturn(args, &scope, GetNumber(isolate, (OPint)camera));
}

static V8Return _GetCameraView(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (IsObject(args, isolate, 1 - inScope, OPscript_MAT4)) {
		OPmat4* view = (OPmat4*)GetArgPointer(args, isolate, 1 - inScope);
		OPcamGetView((*camera), view);
	} else {
		OPlog("No Matrix provided");
	}

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _GetCameraProj(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (IsObject(args, isolate, 1 - inScope, OPscript_MAT4)) {
		OPmat4* proj = (OPmat4*)GetArgPointer(args, isolate, 1 - inScope);
		OPcamGetProj((*camera), proj);
	} else {
		OPlog("No Matrix provided");
	}

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _BindEffect(const V8Args& args) {
	V8Scope scope;

	OPrenderBindEffect((OPeffect*)args[0]->IntegerValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderMesh(const V8Args& args) {
	V8Scope scope;

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

	OPchar* name = (OPchar*)OPalloc(sizeof(OPchar)* (len + 1));
	OPmemcpy(name, p, sizeof(OPchar)*len);
	name[len] = '\0';

	OPmat4* mat = (OPmat4*)args[1]->ToObject()->Get(GetString(isolate, "Id"))->IntegerValue();
	OPrenderParamMat4v(name, 1, mat);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderParamVec3(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParamVec3(p, (OPvec3*)args[1]->IntegerValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _RenderParamTexture(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParami(p, OPtextureBind((OPtexture*)args[1]->IntegerValue()));

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontInit(const V8Args& args) {
	V8Scope scope;

	OPsystemsLoadFontEffect();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerCreate(const V8Args& args) {
	V8Scope scope;

	OPfont* font = (OPfont*)args[0]->IntegerValue();
	OPfontManager* manager = OPfontManagerCreate(font);

	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", GetNumber(isolate, (OPint)manager));
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

	String::Utf8Value str(args[0]->ToString());
	OPfontManagerAddText(ToCString(str));

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontManagerBuild(const V8Args& args) {
	V8Scope scope;

	OPfontManagerBuild();

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontRenderText(const V8Args& args) {
	V8Scope scope;

	String::Utf8Value str(args[0]->ToString());
	OPrenderTextXY(ToCString(str), args[1]->NumberValue(), args[2]->NumberValue());

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FontRenderTextMatrix(const V8Args& args) {
	V8Scope scope; 
	String::Utf8Value str(args[0]->ToString());
	const OPchar* c = ToCString(str);
	Handle<String> el = GetString(isolate, "Id");
	OPrenderTextMat4(c, (OPmat4*)args[1]->ToObject()->Get(el)->IntegerValue());

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
		sprites[i] = (OPsprite*)spritesArray->Get(i)->IntegerValue();
	}

	OPsprite3D* sprite = OPsprite3DCreate(sprites, NULL);

	OPfree(sprites);
	
	V8Object obj = CreateObject(isolate);
	SetValue(isolate, obj, "Id", GetNumber(isolate, (OPint)sprite));
	SetFunction(isolate, obj, "Update", _Sprite3DUpdate);
	SetFunction(isolate, obj, "Render", _Sprite3DRender);
	SetValue(isolate, obj, "Position", GetNumber(isolate, (OPint)&sprite->Position));

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


static V8Return _AudioInit(const V8Args& args) {
	V8Scope scope;

	OPaudInit();
	OPaudInitThread(10);

	OPlog("Audio Initialized");

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _AudioCreate(const V8Args& args) {

	V8Scope scope;

	OPaudioSource* source = (OPaudioSource*)args[0]->IntegerValue();
	OPlog("Source %d", source);
	OPaudioEmitter* emitter = OPaudCreateEmitter(source, EMITTER_THREADED);

	V8Object obj = CreateTypedObject(isolate, emitter, OPscript_AUDIO_EMITTER);
	SetFunction(isolate, obj, "Bind", _AudioBind);
	SetFunction(isolate, obj, "Play", _AudioPlay);
	SetFunction(isolate, obj, "Volume", _AudioVolume);


	OPlog("Emitter Created: %d", emitter);

	return SetReturn(args, &scope, obj);
}

static V8Return _AudioBind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPaudioEmitter* emitter = (OPaudioEmitter*)GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);
	OPaudSetEmitter(emitter);

	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _AudioPlay(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPaudioEmitter* emitter = (OPaudioEmitter*)GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);

	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);

	//OPaudSafePlay(emitter);
	OPaudPlay();
	OPlog("Play audio");
	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _AudioVolume(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPaudioEmitter* emitter = (OPaudioEmitter*)GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);
	//OPaudSetEmitter(emitter);
	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);
	f32 vol = args[1 - inScope]->NumberValue();
	OPlog("Volume set to: %f", vol);
	//OPaudVolume(vol);
	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FrameBufferBind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPframeBuffer* fb = (OPframeBuffer*)GetFirstPointer(args, isolate, &inScope, OPscript_FRAME_BUFFER);
	if(fb == NULL) {
		OPlog("FB is null...");
		return SetReturn(args, &scope, GetNull(isolate));
	}
	OPframeBufferBind(fb);
	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FrameBufferUnbind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPframeBufferUnbind();
	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _FrameBufferCreate(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPtextureDescription desc = {
		OPrenderWidth,
		OPrenderHeight,
		GL_RGBA,
		GL_RGBA,
		GL_FLOAT,
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE
	};
	OPframeBuffer* fb = (OPframeBuffer*)OPalloc(sizeof(OPframeBuffer));
	*fb = OPframeBufferCreate(desc);

	V8Object obj = CreateTypedObject(isolate, fb, OPscript_FRAME_BUFFER);
	SetValue(isolate, obj, "Texture", GetNumber(isolate, (OPint)&fb->Texture));

	return SetReturn(args, &scope, obj);
}

static V8Return _FrameBufferCreateDepth(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPtextureDescription desc = {
		4096,
		4096,
		GL_DEPTH_COMPONENT16,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE
	};
	OPframeBuffer* fb = (OPframeBuffer*)OPalloc(sizeof(OPframeBuffer));
	*fb = OPframeBufferCreateDepth(desc);

	V8Object obj = CreateTypedObject(isolate, fb, OPscript_FRAME_BUFFER);
	SetValue(isolate, obj, "Texture", GetNumber(isolate, (OPint)&fb->Texture));

	return SetReturn(args, &scope, obj);
}

#endif