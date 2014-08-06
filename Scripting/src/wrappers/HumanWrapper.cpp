#include "./Scripting/include/wrappers/HumanWrapper.h"

#ifdef OPIFEX_V8

#include <v8.h>

static void _OPrenderInit(const FunctionCallbackInfo<Value>& args);
static void _Clear(const FunctionCallbackInfo<Value>& args);
static void _Present(const FunctionCallbackInfo<Value>& args);
static void _CreateEffect(const FunctionCallbackInfo<Value>& args);
static void _CreateCamera(const FunctionCallbackInfo<Value>& args);
static void _DestroyCamera(const FunctionCallbackInfo<Value>& args);
static void _GetCameraView(const FunctionCallbackInfo<Value>& args);
static void _GetCameraProj(const FunctionCallbackInfo<Value>& args);

static void _BindMesh(const FunctionCallbackInfo<Value>& args);
static void _BindEffect(const FunctionCallbackInfo<Value>& args);
static void _RenderMesh(const FunctionCallbackInfo<Value>& args);

static void _TextureClearActive(const FunctionCallbackInfo<Value>& args);
static void _RenderParamMat4v(const FunctionCallbackInfo<Value>& args);
static void _RenderParamVec3(const FunctionCallbackInfo<Value>& args);
static void _RenderParamTexture(const FunctionCallbackInfo<Value>& args);

static void _InputUpdate(const v8::FunctionCallbackInfo<v8::Value>& args);
static void _Keyboard(const FunctionCallbackInfo<Value>& args);
static void _Mouse(const FunctionCallbackInfo<Value>& args);


void HumanInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target) {

	// OP
	target->Set(String::NewFromUtf8(isolate, "Clear"), FunctionTemplate::New(isolate, _Clear));
	target->Set(String::NewFromUtf8(isolate, "Present"), FunctionTemplate::New(isolate, _Present));

	// OP.render
	Local<ObjectTemplate> render = ObjectTemplate::New(isolate);
	render->Set(String::NewFromUtf8(isolate, "Init"), FunctionTemplate::New(isolate, _OPrenderInit));
	render->Set(String::NewFromUtf8(isolate, "BindMesh"), FunctionTemplate::New(isolate, _BindMesh));
	render->Set(String::NewFromUtf8(isolate, "BindEffect"), FunctionTemplate::New(isolate, _BindEffect));
	render->Set(String::NewFromUtf8(isolate, "Mesh"), FunctionTemplate::New(isolate, _RenderMesh));
	render->Set(String::NewFromUtf8(isolate, "ParamMat4v"), FunctionTemplate::New(isolate, _RenderParamMat4v));
	render->Set(String::NewFromUtf8(isolate, "ParamVec3"), FunctionTemplate::New(isolate, _RenderParamVec3));
	render->Set(String::NewFromUtf8(isolate, "ParamTexture"), FunctionTemplate::New(isolate, _RenderParamTexture));
	render->Set(String::NewFromUtf8(isolate, "ClearActiveTextures"), FunctionTemplate::New(isolate, _TextureClearActive));
	target->Set(String::NewFromUtf8(isolate, "render"), render);

	// OP.camera
	Local<ObjectTemplate> camera = ObjectTemplate::New(isolate);
	camera->Set(String::NewFromUtf8(isolate, "Create"), FunctionTemplate::New(isolate, _CreateCamera));
	camera->Set(String::NewFromUtf8(isolate, "Destroy"), FunctionTemplate::New(isolate, _DestroyCamera));
	camera->Set(String::NewFromUtf8(isolate, "View"), FunctionTemplate::New(isolate, _GetCameraView));
	camera->Set(String::NewFromUtf8(isolate, "Proj"), FunctionTemplate::New(isolate, _GetCameraProj));
	target->Set(String::NewFromUtf8(isolate, "camera"), camera);

	// OP.effect
	Local<ObjectTemplate> effect = ObjectTemplate::New(isolate);
	effect->Set(String::NewFromUtf8(isolate, "Create"), FunctionTemplate::New(isolate, _CreateEffect));
	target->Set(String::NewFromUtf8(isolate, "effect"), effect);

	// OP.input
	Local<ObjectTemplate> input = ObjectTemplate::New(isolate);
	input->Set(String::NewFromUtf8(isolate, "Update"), FunctionTemplate::New(isolate, _InputUpdate));
	input->Set(String::NewFromUtf8(isolate, "Keyboard"), FunctionTemplate::New(isolate, _Keyboard));
	input->Set(String::NewFromUtf8(isolate, "Mouse"), FunctionTemplate::New(isolate, _Mouse));
	target->Set(String::NewFromUtf8(isolate, "input"), input);

}

#include "./Human/include/Rendering/Renderer.h"
#include "./Scripting/include/Scripting.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Scripting/include/wrappers/V8Helper.h"

void _OPrenderInit(const FunctionCallbackInfo<Value>& args) {
	OPrenderInit(1280, 720, false);

	Local<Function> cb = Local<Function>::Cast(args[0]);
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Boolean::New(isolate, true) };
	cb->Call(Boolean::New(isolate, true), argc, argv);

}
void _Clear(const FunctionCallbackInfo<Value>& args) {
	OPrenderClear(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
}
void _Present(const FunctionCallbackInfo<Value>& args) { 
	OPrenderPresent(); 
}
void _CreateEffect(const FunctionCallbackInfo<Value>& args) {
	Local<Object> obj = args[0]->ToObject();
	Handle<Array> attributesArray = Handle<Array>::Cast(obj->Get(String::NewFromUtf8(isolate, "Attributes")));
	i32 attributeCount = attributesArray->Length();
	OPshaderAttribute* attribs = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute)* attributeCount);
	for (i32 i = 0; i < attributeCount; i++) {
		Local<Object> first = attributesArray->Get(i)->ToObject();
		Local<Value> name = first->Get(String::NewFromUtf8(isolate, "Id"));
		Local<Value> type = first->Get(String::NewFromUtf8(isolate, "Type"));
		Local<Value> size = first->Get(String::NewFromUtf8(isolate, "Size"));

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

	OPshader* vert = (OPshader*)obj->Get(String::NewFromUtf8(isolate, "Vertex"))->Int32Value();
	OPshader* frag = (OPshader*)obj->Get(String::NewFromUtf8(isolate, "Fragment"))->Int32Value();
	const char *effectName = *v8::String::Utf8Value(obj->Get(String::NewFromUtf8(isolate, "Name"))->ToString());

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

	args.GetReturnValue().Set(Integer::New(isolate, (i32)effect));
}

#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/Texture.h"
#include "./Human/include/Input/Input.h"

static void _InputUpdate(const FunctionCallbackInfo<Value>& args) {
	

	OPkeyboardUpdate();
	OPmouseUpdate();

	
}

static void _Mouse(const FunctionCallbackInfo<Value>& args) {
	

	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "X"), Number::New(isolate, OPmousePositionX()));
	obj->Set(String::NewFromUtf8(isolate, "Y"), Number::New(isolate, OPmousePositionY()));
	obj->Set(String::NewFromUtf8(isolate, "dX"), Number::New(isolate, OPmousePositionMovedX()));
	obj->Set(String::NewFromUtf8(isolate, "dY"), Number::New(isolate, OPmousePositionMovedY()));
	obj->Set(String::NewFromUtf8(isolate, "Wheel"), Number::New(isolate, OPmouseWheel()));
	obj->Set(String::NewFromUtf8(isolate, "dWheel"), Number::New(isolate, OPmouseWheelMoved()));
	obj->Set(String::NewFromUtf8(isolate, "LButton"), Number::New(isolate, OPmouseIsDown(OPKEY_LBUTTON)));
	obj->Set(String::NewFromUtf8(isolate, "RButton"), Number::New(isolate, OPmouseIsDown(OPKEY_RBUTTON)));
	obj->Set(String::NewFromUtf8(isolate, "LButtonRelease"), Number::New(isolate, OPmouseWasPressed(OPKEY_LBUTTON)));
	obj->Set(String::NewFromUtf8(isolate, "RButtonRelease"), Number::New(isolate, OPmouseWasPressed(OPKEY_RBUTTON)));

	args.GetReturnValue().Set(obj);
}

static void _Keyboard(const FunctionCallbackInfo<Value>& args) {
	

	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "A"), Number::New(isolate, OPkeyboardIsDown(OPKEY_A)));
	obj->Set(String::NewFromUtf8(isolate, "B"), Number::New(isolate, OPkeyboardIsDown(OPKEY_B)));
	obj->Set(String::NewFromUtf8(isolate, "C"), Number::New(isolate, OPkeyboardIsDown(OPKEY_C)));
	obj->Set(String::NewFromUtf8(isolate, "D"), Number::New(isolate, OPkeyboardIsDown(OPKEY_D)));
	obj->Set(String::NewFromUtf8(isolate, "E"), Number::New(isolate, OPkeyboardIsDown(OPKEY_E)));
	obj->Set(String::NewFromUtf8(isolate, "F"), Number::New(isolate, OPkeyboardIsDown(OPKEY_F)));
	obj->Set(String::NewFromUtf8(isolate, "G"), Number::New(isolate, OPkeyboardIsDown(OPKEY_G)));
	obj->Set(String::NewFromUtf8(isolate, "H"), Number::New(isolate, OPkeyboardIsDown(OPKEY_H)));
	obj->Set(String::NewFromUtf8(isolate, "I"), Number::New(isolate, OPkeyboardIsDown(OPKEY_I)));
	obj->Set(String::NewFromUtf8(isolate, "J"), Number::New(isolate, OPkeyboardIsDown(OPKEY_J)));
	obj->Set(String::NewFromUtf8(isolate, "K"), Number::New(isolate, OPkeyboardIsDown(OPKEY_K)));
	obj->Set(String::NewFromUtf8(isolate, "L"), Number::New(isolate, OPkeyboardIsDown(OPKEY_L)));
	obj->Set(String::NewFromUtf8(isolate, "M"), Number::New(isolate, OPkeyboardIsDown(OPKEY_M)));
	obj->Set(String::NewFromUtf8(isolate, "N"), Number::New(isolate, OPkeyboardIsDown(OPKEY_N)));
	obj->Set(String::NewFromUtf8(isolate, "O"), Number::New(isolate, OPkeyboardIsDown(OPKEY_O)));
	obj->Set(String::NewFromUtf8(isolate, "P"), Number::New(isolate, OPkeyboardIsDown(OPKEY_P)));
	obj->Set(String::NewFromUtf8(isolate, "Q"), Number::New(isolate, OPkeyboardIsDown(OPKEY_Q)));
	obj->Set(String::NewFromUtf8(isolate, "R"), Number::New(isolate, OPkeyboardIsDown(OPKEY_R)));
	obj->Set(String::NewFromUtf8(isolate, "S"), Number::New(isolate, OPkeyboardIsDown(OPKEY_S)));
	obj->Set(String::NewFromUtf8(isolate, "T"), Number::New(isolate, OPkeyboardIsDown(OPKEY_T)));
	obj->Set(String::NewFromUtf8(isolate, "U"), Number::New(isolate, OPkeyboardIsDown(OPKEY_U)));
	obj->Set(String::NewFromUtf8(isolate, "V"), Number::New(isolate, OPkeyboardIsDown(OPKEY_V)));
	obj->Set(String::NewFromUtf8(isolate, "W"), Number::New(isolate, OPkeyboardIsDown(OPKEY_W)));
	obj->Set(String::NewFromUtf8(isolate, "X"), Number::New(isolate, OPkeyboardIsDown(OPKEY_X)));
	obj->Set(String::NewFromUtf8(isolate, "Y"), Number::New(isolate, OPkeyboardIsDown(OPKEY_Y)));
	obj->Set(String::NewFromUtf8(isolate, "Z"), Number::New(isolate, OPkeyboardIsDown(OPKEY_Z)));

	args.GetReturnValue().Set(obj);
}

static void _DestroyCamera(const FunctionCallbackInfo<Value>& args) {
	

	OPcam* camera = (OPcam*)OPalloc(sizeof(OPcam));
	OPfree(camera);

	
}

static void _CreateCamera(const FunctionCallbackInfo<Value>& args) {
	

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
	args.GetReturnValue().Set(Integer::New(isolate, (i32)camera));
}

static void _GetCameraView(const FunctionCallbackInfo<Value>& args) {
	

	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4* view = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPcamGetView((*camera), view);
	args.GetReturnValue().Set(Integer::New(isolate, (i32)view));
}

static void _GetCameraProj(const FunctionCallbackInfo<Value>& args) {
	
	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4* proj = (OPmat4*)OPalloc(sizeof(OPmat4));
	OPcamGetProj((*camera), proj);
	
	args.GetReturnValue().Set(Integer::New(isolate, (i32)proj));
}

static void _BindMesh(const FunctionCallbackInfo<Value>& args) {
	
	OPrenderBindMesh((OPmesh*)args[0]->Int32Value());
	
}

static void _BindEffect(const FunctionCallbackInfo<Value>& args) {
	
	OPrenderBindEffect((OPeffect*)args[0]->Int32Value());
	
}

static void _RenderMesh(const FunctionCallbackInfo<Value>& args) {
	

	OPcam* camera = (OPcam*)args[0]->Int32Value();
	OPmat4 view, proj;

	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);

	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderMesh();
	
}

static void _TextureClearActive(const FunctionCallbackInfo<Value>& args) {
	
	OPtextureClearActive();
	
}
static void _RenderParamMat4v(const FunctionCallbackInfo<Value>& args) {

	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);

	ui32 len = strlen(p);
	//const char *p = *v8::String::Utf8Value(val);

	OPchar* name = (i8*)OPalloc(sizeof(i8)* (len + 1));
	OPmemcpy(name, p, sizeof(i8)*len);
	name[len] = NULL;

	OPmat4* mat = (OPmat4*)args[1]->ToObject()->Get(String::NewFromUtf8(isolate, "Id"))->Int32Value();
	OPrenderParamMat4v(name, 1, mat);
	
}

static void _RenderParamVec3(const FunctionCallbackInfo<Value>& args) {
	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParamVec3(p, 1, (OPvec3*)args[1]->Int32Value());
}

static void _RenderParamTexture(const FunctionCallbackInfo<Value>& args) {
	v8::String::Utf8Value utf8(args[0]);
	const char* p = ToCString(utf8);
	OPrenderParami(p, OPtextureBind((OPtexture*)args[1]->Int32Value()));

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);
}

#endif