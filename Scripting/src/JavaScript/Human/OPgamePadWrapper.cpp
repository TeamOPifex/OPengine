// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

const OPchar* gamePadNames[(ui32)OPgamePadButton::_MAX] = {
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

    for (ui32 i = 0; i < (ui32)OPgamePadButton::_MAX; i++) {
        buttons->Set(
            JS_NEW_STRING(gamePadNames[i]),
            JS_NEW_INTEGER(i)
        );
    }
}

JS_RETURN_VAL _OPgamePadWasPressedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);
    OPgamePadButton btn = (OPgamePadButton)args[0]->IntegerValue();

    JS_RETURN(JS_NEW_BOOL(ptr->WasPressed(btn)));
}

JS_RETURN_VAL _OPgamePadWasReleasedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);
    OPgamePadButton btn = (OPgamePadButton)args[0]->IntegerValue();

    JS_RETURN(JS_NEW_BOOL(ptr->WasReleased(btn)));
}

JS_RETURN_VAL _OPgamePadIsConnected(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_ARG_PTR(args, 0, OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->IsConnected()));
}

JS_RETURN_VAL _OPgamePadIsConnectedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->IsConnected()));
}

JS_RETURN_VAL _OPgamePadIsDownSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);
    OPgamePadButton btn = (OPgamePadButton)args[0]->IntegerValue();

    JS_RETURN(JS_NEW_BOOL(ptr->IsDown(btn)));
}

JS_RETURN_VAL _OPgamePadLeftThumbSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_NUMBER(result, "x", ptr->LeftThumbX());
    JS_SET_NUMBER(result, "y", ptr->LeftThumbY());

    JS_RETURN(result);
}

JS_RETURN_VAL _OPgamePadLeftThumbWasLeftSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbWasLeft()));
}

JS_RETURN_VAL _OPgamePadLeftThumbWasRightSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbWasRight()));
}

JS_RETURN_VAL _OPgamePadLeftThumbNowLeftSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbNowLeft()));
}

JS_RETURN_VAL _OPgamePadLeftThumbNowRightSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbNowRight()));
}


JS_RETURN_VAL _OPgamePadLeftThumbWasUpSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbWasUp()));
}

JS_RETURN_VAL _OPgamePadLeftThumbWasDownSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbWasDown()));
}

JS_RETURN_VAL _OPgamePadLeftThumbNowUpSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbNowUp()));
}

JS_RETURN_VAL _OPgamePadLeftThumbNowDownSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftThumbNowDown()));
}

JS_RETURN_VAL _OPgamePadRightTriggerWasPressedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->RightTriggerWasPressed()));
}

JS_RETURN_VAL _OPgamePadRightTriggerWasReleasedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->RightTriggerWasReleased()));
}

JS_RETURN_VAL _OPgamePadRightTriggerIsDownSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->RightTriggerIsDown()));
}


JS_RETURN_VAL _OPgamePadLeftTriggerWasPressedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftTriggerWasPressed()));
}

JS_RETURN_VAL _OPgamePadLeftTriggerWasReleasedSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftTriggerWasReleased()));
}

JS_RETURN_VAL _OPgamePadLeftTriggerIsDownSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* ptr = JS_GET_PTR(args.This(), OPgamePad);

    JS_RETURN(JS_NEW_BOOL(ptr->LeftTriggerIsDown()));
}

JS_RETURN_VAL _OPgamePadUpdate(const JS_ARGS& args) {
	OPtimer* ptr = JS_GET_PTR(args[0]->ToObject(), OPtimer);
	OPGAMEPADS.Update(ptr);
    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPgamePadSetDeadZones(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

		OPGAMEPADS.SetDeadzones((f32)args[0]->NumberValue());

    JS_RETURN_NULL;
}

void _OPgamePadSetup(Handle<Object> result, OPgamePad* controller) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, controller);
    JS_SET_METHOD(result, "WasPressed", _OPgamePadWasPressedSelf);
    JS_SET_METHOD(result, "WasReleased", _OPgamePadWasReleasedSelf);
    JS_SET_METHOD(result, "IsConnected", _OPgamePadIsConnectedSelf);
    JS_SET_METHOD(result, "LeftThumb", _OPgamePadLeftThumbSelf);
    JS_SET_METHOD(result, "IsDown", _OPgamePadIsDownSelf);
    JS_SET_METHOD(result, "LeftThumbNowLeft", _OPgamePadLeftThumbNowLeftSelf);
    JS_SET_METHOD(result, "LeftThumbNowRight", _OPgamePadLeftThumbNowRightSelf);
    JS_SET_METHOD(result, "LeftThumbWasLeft", _OPgamePadLeftThumbWasLeftSelf);
    JS_SET_METHOD(result, "LeftThumbWasRight", _OPgamePadLeftThumbWasRightSelf);
    JS_SET_METHOD(result, "LeftThumbNowUp", _OPgamePadLeftThumbNowUpSelf);
    JS_SET_METHOD(result, "LeftThumbNowDown", _OPgamePadLeftThumbNowDownSelf);
    JS_SET_METHOD(result, "LeftThumbWasUp", _OPgamePadLeftThumbWasUpSelf);
    JS_SET_METHOD(result, "LeftThumbWasDown", _OPgamePadLeftThumbWasDownSelf);
    JS_SET_METHOD(result, "RightTriggerWasPressed", _OPgamePadRightTriggerWasPressedSelf);
    JS_SET_METHOD(result, "RightTriggerWasReleased", _OPgamePadRightTriggerWasReleasedSelf);
    JS_SET_METHOD(result, "RightTriggerIsDown", _OPgamePadRightTriggerIsDownSelf);
    JS_SET_METHOD(result, "LeftTriggerWasPressed", _OPgamePadLeftTriggerWasPressedSelf);
    JS_SET_METHOD(result, "LeftTriggerWasReleased", _OPgamePadLeftTriggerWasReleasedSelf);
    JS_SET_METHOD(result, "LeftTriggerIsDown", _OPgamePadLeftTriggerIsDownSelf);

}

JS_RETURN_VAL _OPgamePadGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPgamePad* controller = OPGAMEPADS[(i32)args[0]->IntegerValue()];

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
    JS_SET_METHOD(gamepad, "IsConnected", _OPgamePadIsConnected);
    JS_SET_OBJECT(exports, "gamePad", gamepad);

}

#endif
