#include "../../include/wrappers/HumanWrapper.h"

#ifdef OPIFEX_OPTION_V8

#include "./Pipeline/include/OPsprite3D.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Human.h"
#include "./Core/include/OPtimer.h"

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

static V8Return _OP_timer_Create(const V8Args& args);
static V8Return _OP_timer_Update(const V8Args& args);

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
static V8Return _OP_myo_Lock(const V8Args& args);
static V8Return _OP_myo_Unlock(const V8Args& args);

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

OPchar* keyNames[_OPKEYBOARD_MAX] = {
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

OPchar* gamePadNames[_OPGAMEPADBUTTON_MAX] = {
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

V8Object GetKeyboardMapO() {
	V8Object keyboard = OPscriptV8CreateObject(isolate);

	for (OPint i = 0; i < OPKEY_RCONTROL; i++) {
		OPchar* name = keyNames[i];
		OPscriptV8SetValue(isolate, keyboard, name, OPscriptV8GetNumber(isolate, i));
	}

	return keyboard;
}

V8Object GetButtonMapO() {
	V8Object buttons = OPscriptV8CreateObject(isolate);

	for (OPint i = 0; i < _OPGAMEPADBUTTON_MAX; i++) {
		OPchar* name = gamePadNames[i];
		OPscriptV8SetValue(isolate, buttons, name, OPscriptV8GetNumber(isolate, i));
	}

	return buttons;
}

V8Object GetFacingMapO() {
	V8Object facing = OPscriptV8CreateObject(isolate);

	OPscriptV8SetValue(isolate, facing, "LEFT", OPscriptV8GetNumber(isolate, 0));
	OPscriptV8SetValue(isolate, facing, "UP", OPscriptV8GetNumber(isolate, 1));
	OPscriptV8SetValue(isolate, facing, "RIGHT", OPscriptV8GetNumber(isolate, 2));
	OPscriptV8SetValue(isolate, facing, "DOWN", OPscriptV8GetNumber(isolate, 3));

	return facing;
}

void HumanInitializeMethodsO(V8isolate* isolate, V8Object target) {


	// OP
	OPscriptV8SetObject(isolate, target, "KEYS", GetKeyboardMapO());
	OPscriptV8SetObject(isolate, target, "BUTTONS", GetButtonMapO());
	OPscriptV8SetObject(isolate, target, "FACING", GetFacingMapO());

	// OP.render
	V8Object render = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, render, "Clear", _OP_render_Clear);
	OPscriptV8SetFunction(isolate, render, "Blend", _OP_render_Blend);
	OPscriptV8SetFunction(isolate, render, "BlendAlpha", _OP_render_BlendAlpha);
	OPscriptV8SetFunction(isolate, render, "Depth", _OP_render_Depth);
	OPscriptV8SetFunction(isolate, render, "Cull", _OP_render_Cull);
	OPscriptV8SetFunction(isolate, render, "CullMode", _OP_render_CullMode);
	OPscriptV8SetFunction(isolate, render, "Present", _OP_render_Present);
	OPscriptV8SetFunction(isolate, render, "Init", _OP_render_Init);
	OPscriptV8SetFunction(isolate, render, "CreateMesh", _OP_render_CreateMesh);
	OPscriptV8SetFunction(isolate, render, "BuildMesh", _OP_render_BuildMesh);
	OPscriptV8SetFunction(isolate, render, "BindMesh", _OP_render_BindMesh);
	OPscriptV8SetFunction(isolate, render, "BindEffect", _BindEffect);
	OPscriptV8SetFunction(isolate, render, "Mesh", _RenderMesh);
	OPscriptV8SetFunction(isolate, render, "ParamMat4v", _RenderParamMat4v);
	OPscriptV8SetFunction(isolate, render, "ParamVec3", _RenderParamVec3);
	OPscriptV8SetFunction(isolate, render, "ParamTexture", _RenderParamTexture);
	OPscriptV8SetFunction(isolate, render, "ClearActiveTextures", _TextureClearActive);
	OPscriptV8SetFunction(isolate, render, "FullScreen", _OP_render_FullScreen);
	OPscriptV8SetFunction(isolate, render, "ScreenSize", _OP_render_ScreenSize);
	OPscriptV8SetObject(isolate, target, "render", render);

	// OP.camera
	V8Object camera = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, camera, "CreateProj", _CreateProj);
	OPscriptV8SetFunction(isolate, camera, "CreateOrtho", _CreateOrtho);
	OPscriptV8SetFunction(isolate, camera, "Destroy", _DestroyCamera);
	OPscriptV8SetFunction(isolate, camera, "View", _GetCameraView);
	OPscriptV8SetFunction(isolate, camera, "Proj", _GetCameraProj);
	OPscriptV8SetFunction(isolate, camera, "SetPos", _CameraSetPos);
	OPscriptV8SetFunction(isolate, camera, "SetTarget", _CameraSetTarget);
	OPscriptV8SetObject(isolate, target, "camera", camera);

	// OP.effect
	V8Object effect = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, effect, "Create", _OP_effect_Create);
	OPscriptV8SetObject(isolate, target, "effect", effect);

	// OP.timer
	V8Object timer = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, timer, "Create", _OP_timer_Create);
	OPscriptV8SetFunction(isolate, timer, "Update", _OP_timer_Update);
	OPscriptV8SetObject(isolate, target, "timer", timer);

	// OP.input
	V8Object input = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, input, "Update", _InputUpdate);
	OPscriptV8SetFunction(isolate, input, "Keyboard", _Keyboard);
	OPscriptV8SetFunction(isolate, input, "Mouse", _Mouse);
	OPscriptV8SetFunction(isolate, input, "KeyIsDown", _KeyboardIsDown);
	OPscriptV8SetFunction(isolate, input, "KeyIsUp", _KeyboardIsUp);
	OPscriptV8SetFunction(isolate, input, "KeyWasReleased", _KeyboardWasReleased);
	OPscriptV8SetFunction(isolate, input, "KeyWasPressed", _KeyboardWasPressed);
	OPscriptV8SetFunction(isolate, input, "GamePadLeftThumbX", _GamePadLeftThumbX);
	OPscriptV8SetFunction(isolate, input, "GamePadLeftThumbY", _GamePadLeftThumbY);
	OPscriptV8SetFunction(isolate, input, "GamePadLeftThumbIs", _GamePadLeftThumbIs);
	OPscriptV8SetFunction(isolate, input, "GamePadLeftThumbNow", _GamePadLeftThumbNow);
	OPscriptV8SetFunction(isolate, input, "GamePadLeftThumbWas", _GamePadLeftThumbWas);
	OPscriptV8SetFunction(isolate, input, "GamePadRightThumbIs", _GamePadRightThumbIs);
	OPscriptV8SetFunction(isolate, input, "GamePadRightThumbNow", _GamePadRightThumbNow);
	OPscriptV8SetFunction(isolate, input, "GamePadRightThumbWas", _GamePadRightThumbWas);
	OPscriptV8SetFunction(isolate, input, "GamePadRightThumbX", _GamePadRightThumbX);
	OPscriptV8SetFunction(isolate, input, "GamePadRightThumbY", _GamePadRightThumbY);
	OPscriptV8SetFunction(isolate, input, "GamePadIsDown", _GamePadIsDown);
	OPscriptV8SetFunction(isolate, input, "GamePadIsUp", _GamePadIsUp);
	OPscriptV8SetFunction(isolate, input, "GamePadWasPressed", _GamePadWasPressed);
	OPscriptV8SetFunction(isolate, input, "GamePadWasReleased", _GamePadWasReleased);
	OPscriptV8SetFunction(isolate, input, "GamePadIsConnected", _GamePadIsConnected);
	OPscriptV8SetFunction(isolate, input, "GamePadSetDeadzone", _GamePadSetDeadzone);
	OPscriptV8SetObject(isolate, target, "input", input);

	// OP.myo

	V8Object myo = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, myo, "Connect", _OP_myo_Connect);
	OPscriptV8SetFunction(isolate, myo, "Update", _OP_myo_Update);
	OPscriptV8SetFunction(isolate, myo, "Roll", _OP_myo_Roll);
	OPscriptV8SetFunction(isolate, myo, "Pitch", _OP_myo_Pitch);
	OPscriptV8SetFunction(isolate, myo, "Yaw", _OP_myo_Yaw);
	OPscriptV8SetFunction(isolate, myo, "RollChange", _OP_myo_RollChange);
	OPscriptV8SetFunction(isolate, myo, "PitchChange", _OP_myo_PitchChange);
	OPscriptV8SetFunction(isolate, myo, "YawChange", _OP_myo_YawChange);
	OPscriptV8SetFunction(isolate, myo, "SetVibration", _OP_myo_SetVibration);
	OPscriptV8SetFunction(isolate, myo, "PoseNow", _OP_myo_PoseNow);
	OPscriptV8SetFunction(isolate, myo, "PoseWas", _OP_myo_PoseWas);
	OPscriptV8SetFunction(isolate, myo, "PoseIs", _OP_myo_PoseIs);
	OPscriptV8SetFunction(isolate, myo, "Lock", _OP_myo_Lock);
	OPscriptV8SetFunction(isolate, myo, "Unlock", _OP_myo_Unlock);
	OPscriptV8SetObject(isolate, target, "myo", myo);

	// OP.font
	V8Object font = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, font, "Init", _FontInit);
	OPscriptV8SetFunction(isolate, font, "CreateManager", _FontManagerCreate);
	OPscriptV8SetFunction(isolate, font, "RenderText", _FontRenderText);
	OPscriptV8SetFunction(isolate, font, "RenderTextMat4", _FontRenderTextMatrix);
	OPscriptV8SetObject(isolate, target, "font", font);

	// OP.sprite3D
	V8Object sprite3D = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, sprite3D, "Init", _Sprite3DInit);
	OPscriptV8SetFunction(isolate, sprite3D, "Create", _Sprite3DCreate);
	OPscriptV8SetFunction(isolate, sprite3D, "Update", _Sprite3DUpdate);
	OPscriptV8SetFunction(isolate, sprite3D, "Render", _Sprite3DRender);
	OPscriptV8SetObject(isolate, target, "sprite3D", sprite3D);


	// OP.audio
	V8Object audio = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, audio, "Init", _AudioInit);
	OPscriptV8SetFunction(isolate, audio, "Create", _AudioCreate);
	OPscriptV8SetFunction(isolate, audio, "Bind", _AudioBind);
	OPscriptV8SetFunction(isolate, audio, "Play", _AudioPlay);
	OPscriptV8SetFunction(isolate, audio, "Volume", _AudioVolume);
	OPscriptV8SetObject(isolate, target, "audio", audio);


	// OP.frameBuffer	
	V8Object frameBuffer = OPscriptV8CreateObject(isolate);
	OPscriptV8SetFunction(isolate, frameBuffer, "Create", _FrameBufferCreate);
	OPscriptV8SetFunction(isolate, frameBuffer, "CreateDepth", _FrameBufferCreateDepth);
	OPscriptV8SetFunction(isolate, frameBuffer, "Bind", _FrameBufferBind);
	OPscriptV8SetFunction(isolate, frameBuffer, "Unbind", _FrameBufferUnbind);
	OPscriptV8SetObject(isolate, target, "frameBuffer", frameBuffer);
}



static V8Return _OP_render_Init(const V8Args& args) {
	V8Scope scope;

	OPrenderInit();

	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { OPscriptV8GetBool(isolate, true) };
	Handle<Object> obj = OPscriptV8CreateObject(isolate);
	cb->Call(obj, argc, argv);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_Clear(const V8Args& args) {
	V8Scope scope;

	OPrenderClear(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_Blend(const V8Args& args) {
	V8Scope scope;

	OPrenderBlend(args[0]->Int32Value());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_BlendAlpha(const V8Args& args) {
	V8Scope scope;

	OPrenderBlendAlpha();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_Depth(const V8Args& args) {
	V8Scope scope;

	OPrenderDepth(args[0]->Int32Value());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_Cull(const V8Args& args) {
	V8Scope scope;

	OPrenderCull(args[0]->Int32Value());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_CullMode(const V8Args& args) {
	V8Scope scope;

	OPrenderCullMode(args[0]->Int32Value());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_Present(const V8Args& args) {
	V8Scope scope;

	OPrenderPresent();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_CreateMesh(const V8Args& args) {
	V8Scope scope;

	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	*mesh = OPmeshCreate();

	//V8Object obj = OPscriptV8CreateTypedObject(isolate, mesh, OPscript_MESH);
	
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, (OPint)mesh));
}

static V8Return _OP_render_BindMesh(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	//OPmesh* mesh = (OPmesh*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_MESH);
	OPmesh* mesh = (OPmesh*)args[0]->IntegerValue();

	OPmeshBind(mesh);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
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

	OPmeshBuild(
		sizeof(OPfloat) * vertexElementCount,
		sizeof(ui16),
		vertDataCount,
		indexDataCount,
		verts,
		indices);

	OPfree(verts);
	OPfree(indices);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_FullScreen(const V8Args& args) {
	V8Scope scope;

	OPRENDER_FULLSCREEN = args[0]->Int32Value();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_render_ScreenSize(const V8Args& args) {
	V8Scope scope;

	OPRENDER_SCREEN_WIDTH = args[0]->Int32Value();
	OPRENDER_SCREEN_HEIGHT = args[1]->Int32Value();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _OP_effect_Create(const V8Args& args) {
	V8Scope scope;

	Local<Object> obj = args[0]->ToObject();
	Handle<Array> attributesArray = Handle<Array>::Cast(obj->Get(OPscriptV8GetString(isolate, "Attributes")));
	i32 attributeCount = attributesArray->Length();

	OPlog("Attributes found: %d", attributeCount);

	OPshaderAttribute* attribs = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute)* attributeCount);
	for (i32 i = 0; i < attributeCount; i++) {
		Local<Object> first = attributesArray->Get(i)->ToObject();
		Local<Value> name = first->Get(OPscriptV8GetString(isolate, "Id"));
		Local<Value> type = first->Get(OPscriptV8GetString(isolate, "Type"));
		Local<Value> size = first->Get(OPscriptV8GetString(isolate, "Size"));

		v8::String::Utf8Value utf8(name);
		const char* file = OPscriptV8ToCString(utf8);
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

	OPshader* vert = (OPshader*)obj->Get(OPscriptV8GetString(isolate, "Vertex"))->IntegerValue();
	OPshader* frag = (OPshader*)obj->Get(OPscriptV8GetString(isolate, "Fragment"))->IntegerValue();
	const char *effectName = *v8::String::Utf8Value(obj->Get(OPscriptV8GetString(isolate, "Name"))->ToString());

	OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		attributeCount,
		"Model Effect",
		obj->Get(OPscriptV8GetString(isolate, "Stride"))->Int32Value()
		);

	OPfree(attribs);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, (OPint)effect));
}

static V8Return _InputUpdate(const V8Args& args) {
	V8Scope scope;

	// TODO(garrett): make input pass timer
	OPtimer temp = {};
	OPkeyboardUpdate(&temp);
	OPmouseUpdate();
	OPgamePadSystemUpdate();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _Mouse(const V8Args& args) {
	V8Scope scope;

	V8Object obj = OPscriptV8CreateObject(isolate);
	obj->Set(OPscriptV8GetString(isolate, "X"), OPscriptV8GetNumber(isolate, OPmousePositionX()));
	obj->Set(OPscriptV8GetString(isolate, "Y"), OPscriptV8GetNumber(isolate, OPmousePositionY()));
	obj->Set(OPscriptV8GetString(isolate, "dX"), OPscriptV8GetNumber(isolate, OPmousePositionMovedX()));
	obj->Set(OPscriptV8GetString(isolate, "dY"), OPscriptV8GetNumber(isolate, OPmousePositionMovedY()));
	obj->Set(OPscriptV8GetString(isolate, "Wheel"), OPscriptV8GetNumber(isolate, OPmouseWheel()));
	obj->Set(OPscriptV8GetString(isolate, "dWheel"), OPscriptV8GetNumber(isolate, OPmouseWheelMoved()));
	obj->Set(OPscriptV8GetString(isolate, "LButton"), OPscriptV8GetNumber(isolate, OPmouseIsDown(OPMOUSE_LBUTTON)));
	obj->Set(OPscriptV8GetString(isolate, "RButton"), OPscriptV8GetNumber(isolate, OPmouseIsDown(OPMOUSE_RBUTTON)));
	obj->Set(OPscriptV8GetString(isolate, "LButtonRelease"), OPscriptV8GetNumber(isolate, OPmouseWasPressed(OPMOUSE_LBUTTON)));
	obj->Set(OPscriptV8GetString(isolate, "RButtonRelease"), OPscriptV8GetNumber(isolate, OPmouseWasPressed(OPMOUSE_RBUTTON)));

	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _Keyboard(const V8Args& args) {
	V8Scope scope;

	Handle<Array> arr = OPscriptV8CreateArray(isolate);
	for (i32 i = 0; i < _OPKEYBOARD_MAX_VALUE; i++) {
		arr->Set(i, OPscriptV8GetNumber(isolate, OPkeyboardIsDown((OPkeyboardKey)i)));
	}

	return OPscriptV8SetReturn(args, &scope, arr);
}

static V8Return _KeyboardIsDown(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardIsDown((OPkeyboardKey)args[0]->Int32Value());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, result));
}

static V8Return _KeyboardIsUp(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardIsUp((OPkeyboardKey)args[0]->Int32Value());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, result));
}

static V8Return _KeyboardWasReleased(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardWasReleased((OPkeyboardKey)args[0]->Int32Value());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, result));
}

static V8Return _KeyboardWasPressed(const V8Args& args) {
	V8Scope scope;
	bool result = OPkeyboardWasPressed((OPkeyboardKey)args[0]->Int32Value());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, result));
}


static V8Return _OP_myo_Connect(const V8Args& args) {
	V8Scope scope;
	OPmyoConnect();
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, true));
}

static V8Return _OP_myo_Update(const V8Args& args) {
	V8Scope scope;
	OPmyoUpdate(OPgetTime());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, true));
}

static V8Return _OP_myo_Roll(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoRoll()));
}
static V8Return _OP_myo_Pitch(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoPitch()));
}
static V8Return _OP_myo_Yaw(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoYaw()));
}
static V8Return _OP_myo_RollChange(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoRollChange()));
}
static V8Return _OP_myo_PitchChange(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoPitchChange()));
}
static V8Return _OP_myo_YawChange(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPmyoYawChange()));
}
static V8Return _OP_myo_SetVibration(const V8Args& args) {
	V8Scope scope;
	OPmyoVibrations v = (OPmyoVibrations)(args[0]->IntegerValue());
	OPmyoSetVibration(v);
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, true));
}
static V8Return _OP_myo_PoseNow(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPmyoPoseNow(p)));
}
static V8Return _OP_myo_PoseWas(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPmyoPoseWas(p)));
}
static V8Return _OP_myo_PoseIs(const V8Args& args) {
	V8Scope scope;
	OPmyoPoses p = (OPmyoPoses)(args[0]->IntegerValue());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, OPmyoPoseIs(p)));
}
static V8Return _OP_myo_Lock(const V8Args& args) {
	V8Scope scope;
	OPmyoLock();
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, true));
}
static V8Return _OP_myo_Unlock(const V8Args& args) {
	V8Scope scope;
	OPmyoUnlock(args[0]->IntegerValue());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate,true));
}

static V8Return _GamePadLeftThumbX(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPgamePadLeftThumbX(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadLeftThumbY(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPgamePadLeftThumbY(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadLeftThumbIs(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbIsLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbIsUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbIsRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbIsDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadLeftThumbNow(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbNowLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbNowUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbNowRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbNowDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadLeftThumbWas(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbWasLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbWasUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbWasRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadLeftThumbWasDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadRightThumbIs(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbIsLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbIsUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbIsRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbIsDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadRightThumbNow(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbNowLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbNowUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbNowRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbNowDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadRightThumbWas(const V8Args& args) {
	V8Scope scope;
	OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->Int32Value());
	switch (args[1]->Int32Value())
	{
	case 0:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbWasLeft(controller)));
		break;
	case 1:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbWasUp(controller)));
		break;
	case 2:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbWasRight(controller)));
		break;
	case 3:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadRightThumbWasDown(controller)));
		break;
	default:
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
		break;
	}
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, false));
}
static V8Return _GamePadRightThumbX(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPgamePadRightThumbX(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadRightThumbY(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumberF32(isolate, OPgamePadRightThumbY(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()))));
}
static V8Return _GamePadIsDown(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadIsDown(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()), (OPgamePadButton)args[1]->Int32Value())));
}
static V8Return _GamePadIsUp(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadIsUp(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()), (OPgamePadButton)args[1]->Int32Value())));
}
static V8Return _GamePadWasPressed(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadWasPressed(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()), (OPgamePadButton)args[1]->Int32Value())));

}
static V8Return _GamePadWasReleased(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadWasReleased(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()), (OPgamePadButton)args[1]->Int32Value())));

}
static V8Return _GamePadIsConnected(const V8Args& args) {
	V8Scope scope;
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetBool(isolate, OPgamePadIsConnected(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()))));

}
static V8Return _GamePadSetDeadzone(const V8Args& args) {
	V8Scope scope;
	OPgamePadSetDeadzone(OPgamePadGet((OPgamePadIndex)args[0]->Int32Value()), args[1]->NumberValue());
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _DestroyCamera(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	OPfree(camera);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _CreateProj(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = OPcamPersp(
		OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()),
		OPvec3Create(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue()),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	OPvec3Log("Camera Pos", camera->_pos);
	OPvec3Log("Camera Target", camera->_targ);
	OPlog("Aspect: %f", OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT);

	V8Object obj = OPscriptV8CreateTypedObject(isolate, camera, OPscript_CAMERA);
	OPscriptV8SetFunction(isolate, obj, "SetPos", _CameraSetPos);
	OPscriptV8SetFunction(isolate, obj, "SetTarget", _CameraSetTarget);
	OPscriptV8SetFunction(isolate, obj, "View", _GetCameraView);
	OPscriptV8SetFunction(isolate, obj, "Proj", _GetCameraProj);
	//OPscriptV8SetValue(isolate, obj, "Position", (OPint)&camera->_pos);
	
	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _CreateOrtho(const V8Args& args) {
	V8Scope scope;

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = OPcamOrtho(
		OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()),
		OPvec3Create(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue()),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		args[6]->NumberValue(),
		args[7]->NumberValue(),
		args[8]->NumberValue(),
		args[9]->NumberValue()
		);

	V8Object obj = OPscriptV8CreateTypedObject(isolate, camera, OPscript_CAMERA);
	OPscriptV8SetFunction(isolate, obj, "SetPos", _CameraSetPos);
	OPscriptV8SetFunction(isolate, obj, "SetTarget", _CameraSetTarget);
	OPscriptV8SetFunction(isolate, obj, "View", _GetCameraView);
	OPscriptV8SetFunction(isolate, obj, "Proj", _GetCameraProj);
	
	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _CameraSetPos(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (OPscriptV8IsObject(args, isolate, 1 - inScope, OPscript_VEC3)) {
		OPvec3* pos = (OPvec3*)OPscriptV8GetArgPointer(args, isolate, 1 - inScope);
		OPcamSetPosition(camera, (*pos));
	}
	else if(args.Length() > 2) {
		OPvec3 position = OPvec3Create(
			args[1 - inScope]->NumberValue(),
			args[2 - inScope]->NumberValue(),
			args[3 - inScope]->NumberValue());
		OPcamSetPosition(camera, position);
	}

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _CameraSetTarget(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (OPscriptV8IsObject(args, isolate, 1 - inScope, OPscript_VEC3)) {
		OPvec3* target = (OPvec3*)OPscriptV8GetArgPointer(args, isolate, 1 - inScope);
		OPcamSetTarget(camera, (*target));
	}
	else {
		OPvec3 target = OPvec3Create(
			args[1 - inScope]->NumberValue(), 
			args[2 - inScope]->NumberValue(), 
			args[3 - inScope]->NumberValue());
		OPcamSetTarget(camera, target);
	}

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, (OPint)camera));
}

static V8Return _GetCameraView(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (OPscriptV8IsObject(args, isolate, 1 - inScope, OPscript_MAT4)) {
		OPmat4* view = (OPmat4*)OPscriptV8GetArgPointer(args, isolate, 1 - inScope);
		OPcamGetView((*camera), view);
	} else {
		OPlog("No Matrix provided");
	}

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _GetCameraProj(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPcam* camera = (OPcam*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_CAMERA);

	if (OPscriptV8IsObject(args, isolate, 1 - inScope, OPscript_MAT4)) {
		OPmat4* proj = (OPmat4*)OPscriptV8GetArgPointer(args, isolate, 1 - inScope);
		OPcamGetProj((*camera), proj);
	} else {
		OPlog("No Matrix provided");
	}

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _BindEffect(const V8Args& args) {
	V8Scope scope;

	OPeffectBind((OPeffect*)args[0]->IntegerValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _RenderMesh(const V8Args& args) {
	V8Scope scope;

	OPmeshRender();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _TextureClearActive(const V8Args& args) {
	V8Scope scope;

	OPtextureClearActive();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _RenderParamMat4v(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = OPscriptV8ToCString(utf8);
	ui32 len = strlen(p);

	OPchar* name = (OPchar*)OPalloc(sizeof(OPchar)* (len + 1));
	OPmemcpy(name, p, sizeof(OPchar)*len);
	name[len] = '\0';

	OPmat4* mat = (OPmat4*)args[1]->ToObject()->Get(OPscriptV8GetString(isolate, "Id"))->IntegerValue();
	OPeffectParamMat4v(name, 1, mat);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _RenderParamVec3(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = OPscriptV8ToCString(utf8);
	OPeffectParamVec3(p, (OPvec3*)args[1]->IntegerValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _RenderParamTexture(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* p = OPscriptV8ToCString(utf8);
	OPeffectParami(p, OPtextureBind((OPtexture*)args[1]->IntegerValue()));

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontInit(const V8Args& args) {
	V8Scope scope;

	OPfontSystemLoadEffects();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontManagerCreate(const V8Args& args) {
	V8Scope scope;

	OPfont* font = (OPfont*)args[0]->IntegerValue();
	OPfontManager* manager = OPfontManagerCreate(font);

	V8Object obj = OPscriptV8CreateObject(isolate);
	OPscriptV8SetValue(isolate, obj, "Id", OPscriptV8GetNumber(isolate, (OPint)manager));
	OPscriptV8SetFunction(isolate, obj, "Bind", _FontManagerBind);
	OPscriptV8SetFunction(isolate, obj, "SetRGBA", _FontManagerSetRGBA);
	OPscriptV8SetFunction(isolate, obj, "SetAlign", _FontManagerSetAlign);
	OPscriptV8SetFunction(isolate, obj, "AddText", _FontManagerAddText);
	OPscriptV8SetFunction(isolate, obj, "Build", _FontManagerBuild);

	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _FontManagerBind(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)OPscriptV8GetPointer(args, isolate, &inScope, 1);
	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

	OPfontManagerBind(manager);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontManagerSetRGBA(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)OPscriptV8GetPointer(args, isolate, &inScope, 5);
	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

	OPfontManagerSetColor(manager, args[1 - inScope]->NumberValue(), args[2 - inScope]->NumberValue(), args[3 - inScope]->NumberValue(), args[4 - inScope]->NumberValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontManagerSetAlign(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPfontManager* manager = (OPfontManager*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));

	OPfontManagerSetAlign(manager, (OPfontAlign)args[1 - inScope]->Int32Value());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontManagerAddText(const V8Args& args) {
	V8Scope scope;

	String::Utf8Value str(args[0]->ToString());
	OPfontManagerAddText(OPscriptV8ToCString(str));

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontManagerBuild(const V8Args& args) {
	V8Scope scope;

	OPfontManagerBuild();

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontRenderText(const V8Args& args) {
	V8Scope scope;

	String::Utf8Value str(args[0]->ToString());
	OPfontRender(OPscriptV8ToCString(str), args[1]->NumberValue(), args[2]->NumberValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FontRenderTextMatrix(const V8Args& args) {
	V8Scope scope; 
	String::Utf8Value str(args[0]->ToString());
	const OPchar* c = OPscriptV8ToCString(str);
	Handle<String> el = OPscriptV8GetString(isolate, "Id");
	OPfontRender(c, (OPmat4*)args[1]->ToObject()->Get(el)->IntegerValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _Sprite3DInit(const V8Args& args) {
	V8Scope scope;

	OPsprite3DInit(NULL);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
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
	
	V8Object obj = OPscriptV8CreateObject(isolate);
	OPscriptV8SetValue(isolate, obj, "Id", OPscriptV8GetNumber(isolate, (OPint)sprite));
	OPscriptV8SetFunction(isolate, obj, "Update", _Sprite3DUpdate);
	OPscriptV8SetFunction(isolate, obj, "Render", _Sprite3DRender);
	OPscriptV8SetValue(isolate, obj, "Position", OPscriptV8GetNumber(isolate, (OPint)&sprite->Position));

	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _Sprite3DUpdate(const V8Args& args) {
	V8Scope scope;

	i32 result;
	OPsprite3D* sprite = (OPsprite3D*)OPscriptV8GetPointer(args, isolate, &result, 2);
	OPsprite3DUpdate(sprite, args[1 - result]->NumberValue());

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _Sprite3DRender(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPsprite3D* sprite = (OPsprite3D*)OPscriptV8GetPointer(args, isolate, &inScope, 2);
	if (inScope == -1) OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
	OPcam* camera = (OPcam*)OPscriptV8GetArgPointer(args, isolate, 1 - inScope);
	OPsprite3DRender(sprite, camera);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}


static V8Return _AudioInit(const V8Args& args) {
	V8Scope scope;

	OPaudInit();
	OPaudInitThread(10);

	OPlog("Audio Initialized");

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _AudioCreate(const V8Args& args) {

	V8Scope scope;

	OPaudioSource* source = (OPaudioSource*)args[0]->IntegerValue();
	OPlog("Source %d", source);
	OPaudioEmitter* emitter = OPaudCreateEmitter(source, EMITTER_THREADED);

	V8Object obj = OPscriptV8CreateTypedObject(isolate, emitter, OPscript_AUDIO_EMITTER);
	OPscriptV8SetFunction(isolate, obj, "Bind", _AudioBind);
	OPscriptV8SetFunction(isolate, obj, "Play", _AudioPlay);
	OPscriptV8SetFunction(isolate, obj, "Volume", _AudioVolume);


	OPlog("Emitter Created: %d", emitter);

	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _AudioBind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;

	OPaudioEmitter* emitter = (OPaudioEmitter*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);
	OPaudSetEmitter(emitter);

	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _AudioPlay(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPaudioEmitter* emitter = (OPaudioEmitter*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);

	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);

	//OPaudSafePlay(emitter);
	OPaudPlay();
	OPlog("Play audio");
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _AudioVolume(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPaudioEmitter* emitter = (OPaudioEmitter*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_AUDIO_EMITTER);
	//OPaudSetEmitter(emitter);
	OPlog("Current Emitter: %d", OPAUD_CURR_EMITTER);
	f32 vol = args[1 - inScope]->NumberValue();
	OPlog("Volume set to: %f", vol);
	//OPaudVolume(vol);
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FrameBufferBind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPframeBuffer* fb = (OPframeBuffer*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_FRAME_BUFFER);
	if(fb == NULL) {
		OPlog("FB is null...");
		return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
	}
	OPframeBufferBind(fb);
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FrameBufferUnbind(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPframeBufferUnbind();
	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

static V8Return _FrameBufferCreate(const V8Args& args) {
	V8Scope scope;
	i32 inScope;
	OPtextureDescription desc = {
		OPRENDER_WIDTH,
		OPRENDER_HEIGHT,
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

	V8Object obj = OPscriptV8CreateTypedObject(isolate, fb, OPscript_FRAME_BUFFER);
	OPscriptV8SetValue(isolate, obj, "Texture", OPscriptV8GetNumber(isolate, (OPint)&fb->Texture));

	return OPscriptV8SetReturn(args, &scope, obj);
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

	V8Object obj = OPscriptV8CreateTypedObject(isolate, fb, OPscript_FRAME_BUFFER);
	OPscriptV8SetValue(isolate, obj, "Texture", OPscriptV8GetNumber(isolate, (OPint)&fb->Texture));

	return OPscriptV8SetReturn(args, &scope, obj);
}


static V8Return _OP_timer_Elapsed(const V8Args& args) {	

	V8Scope scope;
	i32 inScope;
	OPtimer* timer = (OPtimer*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_TIMER);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, timer->Elapsed));
}

static V8Return _OP_timer_Create(const V8Args& args) {
	V8Scope scope;
	OPtimer* timer = (OPtimer*)OPalloc(sizeof(OPtimer));
	OPtimerCreate(timer);

	V8Object obj = OPscriptV8CreateTypedObject(isolate, timer, OPscript_TIMER);
	OPscriptV8SetFunction(isolate, obj, "Elapsed", _OP_timer_Elapsed);

	return OPscriptV8SetReturn(args, &scope, obj);
}

static V8Return _OP_timer_Update(const V8Args& args) {
	V8Scope scope;

	i32 inScope;
	OPtimer* timer = (OPtimer*)OPscriptV8GetFirstPointer(args, isolate, &inScope, OPscript_TIMER);
	OPtimerTick(timer);

	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
}

#endif