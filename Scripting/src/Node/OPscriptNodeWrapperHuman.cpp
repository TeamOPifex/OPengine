#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"

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
    //obj->Set(NODE_NEW_STRING("ptr"), NODE_NEW_NUMBER((OPint)manager));
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

// OP.input.Keyboard.Update
NODE_RETURN_VAL _OPkeyboardUpdate(const NODE_ARGS& args) {
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
    cam->Set(NODE_NEW_STRING("ptr"), NODE_NEW_NUMBER((OPint)camera));
    NODE_RETURN(cam);
}


NODE_RETURN_VAL _OPfontSystemsLoadEffects(const NODE_ARGS& args) {
    OPfontSystemLoadEffects();
}

NODE_RETURN_VAL _OPfontManagerSetup(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    const OPchar** Names = NULL;
    OPuint TotalEntries = 0;
    OPfontManager* manager = OPfontManagerSetup("Ubuntu.opf", Names, TotalEntries, 1.0);

    Handle<Object> cam = NODE_NEW_OBJECT();
    cam->Set(NODE_NEW_STRING("ptr"), NODE_NEW_NUMBER((OPint)manager));
    NODE_RETURN(cam);
}

NODE_RETURN_VAL _OPfontManagerColor(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();
    OPint ptr = (OPint)obj->Get(NODE_NEW_STRING("ptr"))->IntegerValue();
 	OPfontManager* fontManager = (OPfontManager*)(OPint)ptr;
    OPfontManagerSetColor(fontManager, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPfontRenderBegin(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();
    OPint ptr = (OPint)obj->Get(NODE_NEW_STRING("ptr"))->IntegerValue();
 	OPfontManager* fontManager = (OPfontManager*)(OPint)ptr;

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




void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    SCOPE_AND_ISOLATE


    { // OP.font
        Handle<Object> font = NODE_NEW_OBJECT();
        NODE_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);

        { // OP.font.Manager
            Handle<Object> manager = NODE_NEW_OBJECT();
            NODE_SET_METHOD(manager, "Setup", _OPfontManagerSetup);
            NODE_SET_METHOD(manager, "Color", _OPfontManagerColor);
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


    { // OP.cam
        Handle<Object> cam = NODE_NEW_OBJECT();
        NODE_SET_METHOD(cam, "CreatePersp", _OPcamCreatePersp);
        NODE_SET_OBJECT(exports, "cam", cam);
    }


    {   // OP.input
        Handle<Object> input = NODE_NEW_OBJECT();

        {   // OP.input.Keyboard
            Handle<Object> keyboard = NODE_NEW_OBJECT();
            _SetKeyboardMap(keyboard);
            NODE_SET_METHOD(keyboard, "WasPressed", _OPkeyboardWasPressed);
            NODE_SET_METHOD(keyboard, "WasReleased", _OPkeyboardWasReleased);
            NODE_SET_METHOD(keyboard, "Update", _OPkeyboardUpdate);
            NODE_SET_OBJECT(input, "Keyboard", keyboard);
        }

        {   // OP.input.GamePad
            Handle<Object> gamepad = NODE_NEW_OBJECT();
            _SetGamePadMap(gamepad);
            NODE_SET_OBJECT(input, "GamePad", gamepad);
        }

        NODE_SET_OBJECT(exports, "input", input);
    }


    {   // OP.render
        Handle<Object> render = NODE_NEW_OBJECT();
        NODE_SET_METHOD(render, "Init", _OPrenderInit);
        NODE_SET_METHOD(render, "Clear", _OPrenderClear);
        NODE_SET_METHOD(render, "Present", _OPrenderPresent);
        NODE_SET_METHOD(render, "Size", _OPrenderSize);
        NODE_SET_OBJECT(exports, "render", render);
    }

}

#endif