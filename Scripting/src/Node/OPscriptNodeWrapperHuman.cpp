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
 		    String::NewFromUtf8(isolate, keyNames[i]),
 		    Number::New(isolate, i)
 		);
 	}
 }

void _SetGamePadMap(Handle<Object> buttons) {
    SCOPE_AND_ISOLATE

 	for (OPint i = 0; i < _OPGAMEPADBUTTON_MAX; i++) {
 		buttons->Set(
 		    String::NewFromUtf8(isolate, gamePadNames[i]),
 		    Number::New(isolate, i)
 		);
 	}
}

// OP.render.Init
void _OPrenderInit(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderInit();
}

// OP.render.Clear
void _OPrenderClear(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderClear(
        args[0]->NumberValue(),
        args[1]->NumberValue(),
        args[2]->NumberValue());
}

// OP.render.Present
void _OPrenderPresent(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE
    OPrenderPresent();
}

// OP.input.Keyboard.WasPressed
void _OPkeyboardWasPressed(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasPressed(key);

    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

// OP.input.Keyboard.WasReleased
void _OPkeyboardWasReleased(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasReleased(key);

    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

// OP.input.Keyboard.Update
void _OPkeyboardUpdate(const FunctionCallbackInfo<Value>& args) {
    OPkeyboardUpdate(NULL);
}

void _OPcamCreatePersp(const FunctionCallbackInfo<Value>& args) {
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

    Handle<Object> cam = Object::New(isolate);
    cam->Set(String::NewFromUtf8(isolate, "ptr"), Number::New(isolate, (OPint)camera));
    args.GetReturnValue().Set(cam);
}


void _OPfontSystemsLoadEffects(const FunctionCallbackInfo<Value>& args) {
    OPfontSystemLoadEffects();
}

void _OPfontManagerSetup(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

    OPfontManager* manager = OPfontManagerSetup("Ubuntu.opf", Names, TotalEntries, 0.5);

}

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    SCOPE_AND_ISOLATE


    { // OP.font
        Handle<Object> font = Object::New(isolate);
        NODE_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);

        { // OP.font.Manager
            Handle<Object> manager = Object::New(isolate);
            NODE_SET_METHOD(manager, "Setup", _OPfontManagerSetup);
            font->Set(String::NewFromUtf8(isolate, "Manager"), manager);
        }

        exports->Set(String::NewFromUtf8(isolate, "font"), font);
    }


    { // OP.cam
        Handle<Object> cam = Object::New(isolate);
        NODE_SET_METHOD(cam, "CreatePersp", _OPcamCreatePersp);
        exports->Set(String::NewFromUtf8(isolate, "cam"), cam);
    }


    {   // OP.input
        Handle<Object> input = Object::New(isolate);

        {   // OP.input.Keyboard
            Handle<Object> keyboard = Object::New(isolate);
            _SetKeyboardMap(keyboard);
            NODE_SET_METHOD(keyboard, "WasPressed", _OPkeyboardWasPressed);
            NODE_SET_METHOD(keyboard, "WasReleased", _OPkeyboardWasReleased);
            NODE_SET_METHOD(keyboard, "Update", _OPkeyboardUpdate);
            input->Set(String::NewFromUtf8(isolate, "Keyboard"), keyboard);
        }

        {   // OP.input.GamePad
            Handle<Object> gamepad = Object::New(isolate);
            _SetGamePadMap(gamepad);
            input->Set(String::NewFromUtf8(isolate, "GamePad"), gamepad);
        }

        NODE_SET_OBJECT(exports, "input", input);
    }


    {   // OP.render
        Handle<Object> render = Object::New(isolate);
        NODE_SET_METHOD(render, "Init", _OPrenderInit);
        NODE_SET_METHOD(render, "Clear", _OPrenderClear);
        NODE_SET_METHOD(render, "Present", _OPrenderPresent);
        NODE_SET_OBJECT(exports, "render", render);
    }

}

#endif