// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

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

void _SetGamePadMap(Handle<Object> buttons) {
    SCOPE_AND_ISOLATE

    for (OPint i = 0; i < _OPGAMEPADBUTTON_MAX; i++) {
        buttons->Set(
            NODE_NEW_STRING(gamePadNames[i]),
            NODE_NEW_NUMBER(i)
        );
    }
}

void OPgamePadWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> gamepad = NODE_NEW_OBJECT();
    _SetGamePadMap(gamepad);
    NODE_SET_OBJECT(exports, "gamePad", gamepad);

}