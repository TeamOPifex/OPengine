#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/Math/OPvec3Wrapper.h"
#include "./Scripting/include/Node/Math/OPmat4Wrapper.h"
#include "./Scripting/include/Node/Human/OPcamWrapper.h"

#include "./Core/include/OPlog.h"
#include "./Human/Human.h"
#include "./Human/include/Systems/OPinputSystem.h"

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

void _SetKeyboardMap(Handle<Object> keyboard) {
    SCOPE_AND_ISOLATE

 	for (OPint i = 0; i < OPKEY_RCONTROL; i++) {
 		keyboard->Set(
 		    NODE_NEW_STRING(keyNames[i]),
 		    NODE_NEW_NUMBER(i)
 		);
 	}
 }

void _SetGamePadMap(Handle<Object> buttons) {
    SCOPE_AND_ISOLATE

 	for (OPint i = 0; i < _OPGAMEPADBUTTON_MAX; i++) {
 		buttons->Set(
 		    NODE_NEW_STRING(gamePadNames[i]),
 		    NODE_NEW_NUMBER(i)
 		);
 	}
}

// OP.render.Init
NODE_RETURN_VAL _OPrenderInit(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPrenderInit();

    NODE_RETURN_NULL
}

// OP.render.Clear
NODE_RETURN_VAL _OPrenderClear(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPrenderClear(
        args[0]->NumberValue(),
        args[1]->NumberValue(),
        args[2]->NumberValue());

    NODE_RETURN_NULL
}

// OP.render.Present
NODE_RETURN_VAL _OPrenderPresent(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPrenderPresent();

    NODE_RETURN_NULL
}

// OP.render.Size
NODE_RETURN_VAL _OPrenderSize(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = NODE_NEW_OBJECT();
    NODE_SET_NUMBER(obj, "Width", OPRENDER_WIDTH);
    NODE_SET_NUMBER(obj, "Height", OPRENDER_HEIGHT);
    NODE_SET_NUMBER(obj, "ScreenWidth", OPRENDER_SCREEN_WIDTH);
    NODE_SET_NUMBER(obj, "ScreenHeight", OPRENDER_SCREEN_HEIGHT);
    NODE_SET_NUMBER(obj, "ScaledWidth", OPRENDER_SCALED_WIDTH);
    NODE_SET_NUMBER(obj, "ScaledHeight", OPRENDER_SCALED_HEIGHT);

    NODE_RETURN(obj);
}

// OP.input.Keyboard.WasPressed
NODE_RETURN_VAL _OPkeyboardWasPressed(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasPressed(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}

// OP.input.Keyboard.WasReleased
NODE_RETURN_VAL _OPkeyboardWasReleased(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasReleased(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}

// OP.input.Keyboard.IsDown
NODE_RETURN_VAL _OPkeyboardIsDown(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardIsDown(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}


// OP.input.Keyboard.Update
NODE_RETURN_VAL _OPkeyboardUpdate(const NODE_ARGS& args) {
 	SCOPE_AND_ISOLATE

    OPkeyboardUpdate(NULL);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPcamCreatePersp(const NODE_ARGS& args) {
 	SCOPE_AND_ISOLATE

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

    Handle<Object> cam = NODE_NEW_OBJECT();
    NODE_SET_PTR(cam, camera);

    NODE_RETURN(cam);
}


NODE_RETURN_VAL _OPfontSystemsLoadEffects(const NODE_ARGS& args) {
    OPfontSystemLoadEffects();
}

NODE_RETURN_VAL _OPfontManagerScale(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	OPfontManager* fontManager = NODE_GET_PTR(args.This(), OPfontManager);
    fontManager->scale = args[0]->NumberValue();

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontManagerSetup(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfontManager* manager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, manager);
    NODE_SET_METHOD(result, "scale", _OPfontManagerScale);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPfontManagerColor(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	OPfontManager* fontManager = NODE_GET_ARG_PTR(args, 0, OPfontManager);
    OPfontManagerSetColor(fontManager, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontRenderBegin(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();
 	OPfontManager* fontManager = NODE_GET_PTR(obj, OPfontManager);
 	fontManager->scale = NODE_GET_NUMBER(obj, "scale");
    OPfontRenderBegin(fontManager);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontRenderColor(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfontColor(OPvec4Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0));

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontRenderText(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    String::Utf8Value str(args[0]->ToString());
    OPfontRender(*str, OPvec2Create(args[1]->NumberValue(), args[2]->NumberValue()));

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontRenderEnd(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfontRenderEnd();

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPrenderDepth(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPrenderDepth(args[0]->IntegerValue());

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmeshCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    *mesh = OPmeshCreate();
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, mesh);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPmeshDestroy(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshDestroy(mesh);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmeshBuild(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

//    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
//
//    OPmeshBuild(
//        ui32 vertSize,
//        ui32 indSize,
//        ui32 vertCount,
//        ui32 indCount,
//        void* vertices,
//        void* indices);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmeshBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmesh* mesh = NODE_GET_ARG_PTR(args, 0, OPmesh);
    OPmeshBind(mesh);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmeshRender(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmeshRender();

    NODE_RETURN_NULL
}


NODE_RETURN_VAL _OPmaterialClearParams(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialClearParams(material);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmaterialInit(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPeffect* effect = NODE_GET_ARG_PTR(args, 1, OPeffect);
    OPmaterialInit(material, effect);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmaterialCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
    OPeffect* effect = NODE_GET_ARG_PTR(args, 0, OPeffect);
    OPmaterialInit(material, effect);

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, material);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPmaterialAddParam(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    enum OPmaterialParamType matParamType = (enum OPmaterialParamType)(OPint)args[1]->NumberValue();
    String::Utf8Value str(args[2]->ToString());
    void* data = (void*)(OPint)args[3]->NumberValue();
    ui8 count = (ui8)args[4]->NumberValue();
    OPmaterialAddParam(material, matParamType, *str, data, count);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPmaterialBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialBind(material);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPcubeCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    *mesh = OPcubeCreate(OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()));

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, mesh);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPeffectGen(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));

    String::Utf8Value vert(args[0]->ToString());
    String::Utf8Value frag(args[1]->ToString());
    ui32 attrs = args[2]->IntegerValue();
    String::Utf8Value name(args[3]->ToString());
    ui32 vertSize = args[4]->IntegerValue();
    *effect = OPeffectGen(
        *vert,
        *frag,
        attrs,
        *name,
        vertSize
        );

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, effect);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPeffectBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPeffect* effect = NODE_GET_ARG_PTR(args, 0, OPeffect);
    OPeffectBind(effect);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPeffectParamMat4(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE


    String::Utf8Value param(args[0]->ToString());
    OPmat4* obj = NODE_GET_ARG_PTR(args, 1, OPmat4);
    OPeffectParamMat4(*param, obj);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPeffectParamVec3(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE


    String::Utf8Value param(args[0]->ToString());
    OPvec3* obj = NODE_GET_ARG_PTR(args, 1, OPvec3);
    OPeffectParamVec3(*param, obj);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPeffectParamTexture(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    String::Utf8Value param(args[0]->ToString());
    OPtexture* ptr = NODE_GET_ARG_PTR(args, 1, OPtexture);
    OPeffectParam(*param, ptr);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPeffectParamCam(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPcam* obj = NODE_GET_ARG_PTR(args, 0, OPcam);
    OPeffectParam(*obj);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPtextureClearActive(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtextureClearActive();

    NODE_RETURN_NULL
}

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    SCOPE_AND_ISOLATE


    { // OP.font
        Handle<Object> font = NODE_NEW_OBJECT();
        NODE_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);

        { // OP.font.Manager
            Handle<Object> manager = NODE_NEW_OBJECT();
            NODE_SET_METHOD(manager, "Setup", _OPfontManagerSetup);
            NODE_SET_METHOD(manager, "Color", _OPfontManagerColor);
            NODE_SET_NUMBER(manager, "size", sizeof(OPfontManager));
            NODE_SET_OBJECT(font, "Manager", manager);
        }

        { // OP.font.Render
            Handle<Object> render = NODE_NEW_OBJECT();
            NODE_SET_METHOD(render, "Begin", _OPfontRenderBegin);
            NODE_SET_METHOD(render, "Color", _OPfontRenderColor);
            NODE_SET_METHOD(render, "Text", _OPfontRenderText);
            NODE_SET_METHOD(render, "End", _OPfontRenderEnd);
            NODE_SET_OBJECT(font, "Render", render);
        }

        NODE_SET_OBJECT(exports, "font", font);
    }


    { // OP.mesh
        Handle<Object> mesh = NODE_NEW_OBJECT();
        NODE_SET_METHOD(mesh, "Create", _OPmeshCreate);
        NODE_SET_METHOD(mesh, "Destroy", _OPmeshDestroy);
        NODE_SET_METHOD(mesh, "Build", _OPmeshBuild);
        NODE_SET_METHOD(mesh, "Bind", _OPmeshBind);
        NODE_SET_METHOD(mesh, "Render", _OPmeshRender);
        NODE_SET_NUMBER(mesh, "size", sizeof(OPmesh));
        NODE_SET_OBJECT(exports, "mesh", mesh);
    }

    { // OP.material
        Handle<Object> material = NODE_NEW_OBJECT();
        NODE_SET_METHOD(material, "ClearParams", _OPmaterialClearParams);
        NODE_SET_METHOD(material, "Init", _OPmaterialInit);
        NODE_SET_METHOD(material, "Create", _OPmaterialCreate);
        NODE_SET_METHOD(material, "AddParam", _OPmaterialAddParam);
        NODE_SET_METHOD(material, "Bind", _OPmaterialBind);
        NODE_SET_NUMBER(material, "size", sizeof(OPmaterial));
        NODE_SET_OBJECT(exports, "material", material);
    }


//    { // OP.cam
//        Handle<Object> cam = NODE_NEW_OBJECT();
//        NODE_SET_METHOD(cam, "CreatePersp", _OPcamCreatePersp);
//        NODE_SET_NUMBER(cam, "size", sizeof(OPcam));
//        NODE_SET_OBJECT(exports, "cam", cam);
//    }


    {   // OP.keyboard
        Handle<Object> keyboard = NODE_NEW_OBJECT();
        _SetKeyboardMap(keyboard);
        NODE_SET_METHOD(keyboard, "WasPressed", _OPkeyboardWasPressed);
        NODE_SET_METHOD(keyboard, "WasReleased", _OPkeyboardWasReleased);
        NODE_SET_METHOD(keyboard, "IsDown", _OPkeyboardIsDown);
        NODE_SET_METHOD(keyboard, "Update", _OPkeyboardUpdate);
        NODE_SET_OBJECT(exports, "keyboard", keyboard);
    }


    {   // OP.gamePad
        Handle<Object> gamepad = NODE_NEW_OBJECT();
        _SetGamePadMap(gamepad);
        NODE_SET_OBJECT(exports, "gamePad", gamepad);
    }


    { // OP.cube
        Handle<Object> cube = NODE_NEW_OBJECT();
        NODE_SET_METHOD(cube, "Create", _OPcubeCreate);
        NODE_SET_OBJECT(exports, "cube", cube);
    }


    { // OP.effect
        Handle<Object> effect = NODE_NEW_OBJECT();
        NODE_SET_METHOD(effect, "Gen", _OPeffectGen);
        NODE_SET_METHOD(effect, "Bind", _OPeffectBind);
        NODE_SET_METHOD(effect, "ParamMat4", _OPeffectParamMat4);
        NODE_SET_METHOD(effect, "ParamVec3", _OPeffectParamVec3);
        NODE_SET_METHOD(effect, "ParamTex", _OPeffectParamTexture);
        NODE_SET_METHOD(effect, "ParamCam", _OPeffectParamCam);
        NODE_SET_OBJECT(exports, "effect", effect);
    }


    {   // OP.render
        Handle<Object> render = NODE_NEW_OBJECT();
        NODE_SET_METHOD(render, "Init", _OPrenderInit);
        NODE_SET_METHOD(render, "Clear", _OPrenderClear);
        NODE_SET_METHOD(render, "Present", _OPrenderPresent);
        NODE_SET_METHOD(render, "Size", _OPrenderSize);
        NODE_SET_METHOD(render, "Depth", _OPrenderDepth);
        NODE_SET_OBJECT(exports, "render", render);
    }

    { // OP.texture
        Handle<Object> texture = NODE_NEW_OBJECT();
        NODE_SET_METHOD(texture, "ClearActive", _OPtextureClearActive);
        NODE_SET_OBJECT(exports, "texture", texture);
    }

    OPcamWrapper(exports);

    NODE_SET_NUMBER(exports, "ATTR_POSITION", OPATTR_POSITION);
    NODE_SET_NUMBER(exports, "ATTR_UV", OPATTR_UV);
    NODE_SET_NUMBER(exports, "ATTR_TANGENT", OPATTR_TANGENT);
    NODE_SET_NUMBER(exports, "ATTR_NORMAL", OPATTR_NORMAL);
    NODE_SET_NUMBER(exports, "ATTR_COLOR", OPATTR_COLOR);
    NODE_SET_NUMBER(exports, "ATTR_BONES", OPATTR_BONES);
    NODE_SET_NUMBER(exports, "ATTR_COLOR4", OPATTR_COLOR4);
}

#endif