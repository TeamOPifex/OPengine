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

JS_RETURN_VAL _OPgamePadWasPressed(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);
    OPgamePadButton btn = (OPgamePadButton)args[0]->IntegerValue();

    JS_RETURN(JS_NEW_BOOL(OPgamePadWasPressed(ptr, btn)));
}

JS_RETURN_VAL _OPgamePadUpdate(const JS_ARGS& args) {
    OPgamePadSystemUpdate();
}

JS_RETURN_VAL _OPgamePadSetDeadZones(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePadSetDeadZones(args[0]->NumberValue());

    JS_RETURN_NULL;
}

void _OPgamePadSetup(Handle<Object> result, OPgamePad* controller) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, controller);
    JS_SET_METHOD(result, "WasPressed", _OPgamePadWasPressed);

}

JS_RETURN_VAL _OPgamePadGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* controller = OPgamePadGet((OPgamePadIndex)args[0]->IntegerValue());

    Handle<Object> result = JS_NEW_OBJECT();
    _OPgamePadSetup(result, controller);

    JS_RETURN(result);
}

void OPgamePadWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> gamepad = JS_NEW_OBJECT();
    _SetGamePadMap(gamepad);
    JS_SET_METHOD(gamepad, "Get", _OPgamePadGet);
    JS_SET_METHOD(gamepad, "Update", _OPgamePadUpdate);
    JS_SET_METHOD(gamepad, "SetDeadZones", _OPgamePadSetDeadZones);
    JS_SET_OBJECT(exports, "gamePad", gamepad);

}

#endif