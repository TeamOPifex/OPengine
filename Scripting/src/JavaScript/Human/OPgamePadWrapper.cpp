// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

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
            JS_NEW_STRING(gamePadNames[i]),
            JS_NEW_NUMBER(i)
        );
    }
}

void OPgamePadWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> gamepad = JS_NEW_OBJECT();
    _SetGamePadMap(gamepad);
    JS_SET_OBJECT(exports, "gamePad", gamepad);

}

#endif