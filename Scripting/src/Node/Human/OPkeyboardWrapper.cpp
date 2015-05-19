// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"


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

void _SetKeyboardMap(Handle<Object> keyboard) {
    SCOPE_AND_ISOLATE;

    for (OPint i = 0; i < OPKEY_RCONTROL; i++) {
        keyboard->Set(
            NODE_NEW_STRING(keyNames[i]),
            NODE_NEW_NUMBER(i)
        );
    }
}



// OP.input.Keyboard.WasPressed
NODE_RETURN_VAL _OPkeyboardWasPressed(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasPressed(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}

// OP.input.Keyboard.WasReleased
NODE_RETURN_VAL _OPkeyboardWasReleased(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardWasReleased(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}

// OP.input.Keyboard.IsDown
NODE_RETURN_VAL _OPkeyboardIsDown(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    enum OPkeyboardKey key = (enum OPkeyboardKey)args[0]->Int32Value();
    bool result = OPkeyboardIsDown(key);

    NODE_RETURN(NODE_NEW_BOOL(result));
}


// OP.input.Keyboard.Update
NODE_RETURN_VAL _OPkeyboardUpdate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

            OPkeyboardUpdate(NULL);

    NODE_RETURN_NULL;
}

void OPkeyboardWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> keyboard = NODE_NEW_OBJECT();
    NODE_SET_METHOD(keyboard, "WasPressed", _OPkeyboardWasPressed);
    NODE_SET_METHOD(keyboard, "WasReleased", _OPkeyboardWasReleased);
    NODE_SET_METHOD(keyboard, "IsDown", _OPkeyboardIsDown);
    NODE_SET_METHOD(keyboard, "Update", _OPkeyboardUpdate);
    NODE_SET_OBJECT(exports, "keyboard", keyboard);


    Handle<Object> keys = NODE_NEW_OBJECT();
    _SetKeyboardMap(keys);
    NODE_SET_OBJECT(exports, "KEY", keys);
}