#include "./Human/include/Input/OPgamePad.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS) && defined(OPIFEX_UNIX)
	//#include <GLFW/glfw3.h>
#elif defined(OPIFEX_WINDOWS)
	#include <Xinput.h>
	#pragma comment(lib, "XInput.lib")
#elif defined(OPIFEX_ANDROID)
	#include "./Core/include/OPcore.h"
	#include "./Human/include/Utilities/AndroidNDK.h"
#endif

OPgamePadSystem OPGAMEPADSYSTEM = {
	OPgamePad(0),
	OPgamePad(1),
	OPgamePad(2),
	OPgamePad(3)
};

//OPgamePad GamePads[CONTROLLERS] = {
//	{
//		0,
//		false,
//		0.1f
//	},
//	{
//		1,
//		false,
//		0.1f
//	},
//	{
//		2,
//		false,
//		0.1f
//	},
//	{
//		3,
//		false,
//		0.1f
//	}
//};

//-----------------------------------------------------------------------------
//OPgamePad* OPgamePadGet(OPgamePadIndex index) {
//	return &GamePads[index];
//}
//-----------------------------------------------------------------------------
#ifdef OPIFEX_ANDROID

	#define CONTROLLER_CLASS_NAME "tv/ouya/console/api/OuyaController"

	jobject OPjniGetControllerByPlayer(OPint playerNum) {
		OPJniMethodInfo methodInfo;
		if (!OPjniGetStaticMethodInfo(methodInfo,
			CONTROLLER_CLASS_NAME, "getControllerByPlayer",
			"(I)Ltv/ouya/console/api/OuyaController;")) {
				return 0;
		}
		return methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
			methodInfo.methodID, playerNum);
	}


	OPint OPjniGetControllerButton(jobject controller, OPint button) {

		OPJniMethodInfo methodInfo2;
		if (!OPjniGetMethodInfo(methodInfo2,
			CONTROLLER_CLASS_NAME, "getButton", "(I)Z")) {
				OPlog("Class method not found");
				return false;
		}

		JNIEnvironment()->DeleteLocalRef(methodInfo2.classID);

		OPint result = JNIEnvironment()->CallBooleanMethod(controller, methodInfo2.methodID, button);

		if(result) {
			OPlog("Button Pressed: %d", button);
		} else {

		}

		return result;
	}

	OPfloat OPjniGetAxisValue( jobject controller, OPint ouyaAxis ) {

		OPJniMethodInfo methodInfo;

		if (!OPjniGetMethodInfo(methodInfo, CONTROLLER_CLASS_NAME, "getAxisValue", "(I)F"))
		{
			return 0;
		}

		jfloat f = methodInfo.env->CallFloatMethod(controller, methodInfo.methodID, ouyaAxis);

		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return f;
	}

#endif


//-----------------------------------------------------------------------------
#ifdef OPIFEX_ANDROID
void __OPandUpdateGamePad(OPgamePad* controller){
	jobject jcontroller = OPjniGetControllerByPlayer(controller->playerIndex);

	if(jcontroller == 0) {
		return;
	}

	controller->connected = true;

	controller->buttons[OPGAMEPADBUTTON_A] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_O);
	controller->buttons[OPGAMEPADBUTTON_X] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_U);
	controller->buttons[OPGAMEPADBUTTON_B] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_A);
	controller->buttons[OPGAMEPADBUTTON_Y] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_Y);

	controller->buttons[OPGAMEPADBUTTON_LEFT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L1);
	controller->buttons[OPGAMEPADBUTTON_RIGHT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R1);

	controller->buttons[OPGAMEPADBUTTON_LEFT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L3);
	controller->buttons[OPGAMEPADBUTTON_RIGHT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R3);


	controller->buttons[OPGAMEPADBUTTON_DPAD_UP] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_UP);
	controller->buttons[OPGAMEPADBUTTON_DPAD_DOWN] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_DOWN);
	controller->buttons[OPGAMEPADBUTTON_DPAD_LEFT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_LEFT);
	controller->buttons[OPGAMEPADBUTTON_DPAD_RIGHT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_RIGHT);

	controller->buttons[OPGAMEPADBUTTON_START] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_MENU);

	controller->axes[OPGAMEPADAXIS_L2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_L2);
	controller->axes[OPGAMEPADAXIS_R2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_R2);

	controller->axes[OPGAMEPADAXIS_LS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_X);
	controller->axes[OPGAMEPADAXIS_LS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_Y);

	controller->axes[OPGAMEPADAXIS_RS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_X);
	controller->axes[OPGAMEPADAXIS_RS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_Y);
}
#endif
//-----------------------------------------------------------------------------
#if defined(OPIFEX_UNIX) && !defined(OPIFEX_IOS)
void __OPlnxUpdateGamePad(OPgamePad* c){

	i32  axes = 0, buttons = 0;
	const f32* axisData   = NULL;
	const ui8* buttonData = NULL;

	// make sure the gamepad is connected
	if(!glfwJoystickPresent((i32)c->controllerIndex)){
		//OPlog("Controller %d not connected", (i32)c->playerIndex);
		c->connected = 0;
		return;
	}

	c->connected = true;

	// get axis data, andmake sure the number of axes are expected
	axisData = glfwGetJoystickAxes(c->controllerIndex, &axes);
	//OPlog("Axes %d", axes);
	// if(axes != 8){
	// 	// game pads should have 8 axes
	// 	// 2 for each stick, and 2 for the dpad
	// 	c->connected = 0;
	// 	return;
	// }

	// map left stick and trigger
	c->axes[OPGAMEPADAXIS_LS_X] =  axisData[0];
	c->axes[OPGAMEPADAXIS_LS_Y] = -axisData[1];

	c->axes[OPGAMEPADAXIS_RS_X] =  axisData[2];
	c->axes[OPGAMEPADAXIS_RS_Y] = -axisData[3];

	c->axes[OPGAMEPADAXIS_L2]   =  (axisData[4] + 1.0f) / 2.0f;
	c->axes[OPGAMEPADAXIS_R2]   =  (axisData[5] + 1.0f) / 2.0f;

	// OPlog("L %f %f %f", c->axes[OPGAMEPADAXIS_LS_X], c->axes[OPGAMEPADAXIS_LS_Y], c->axes[OPGAMEPADAXIS_L2]);
	// OPlog("R %f %f %f", c->axes[OPGAMEPADAXIS_RS_X], c->axes[OPGAMEPADAXIS_RS_Y], c->axes[OPGAMEPADAXIS_R2]);

	// if(axes == 8) {
	// 	// map right stick and trigger
	// 	c->axes[OPGAMEPADAXIS_RS_X] =  axisData[3];
	// 	c->axes[OPGAMEPADAXIS_RS_Y] = -axisData[4];
	// 	c->axes[OPGAMEPADAXIS_R2]   =  (axisData[5] + 1.0f) / 2.0f;
	// }


	// get button data, make sure it's all kosher
	buttonData = glfwGetJoystickButtons(c->controllerIndex, &buttons);
	if(buttons < 10){
		// game pads should have 10 buttons
		OPlog("GamePad had %d buttons but has to have at least %d buttons", buttons, 10);
		c->connected = 0;
		return;
	}

	c->buttons[OPGAMEPADBUTTON_DPAD_LEFT]  = buttonData[2];
	c->buttons[OPGAMEPADBUTTON_DPAD_RIGHT] = buttonData[3];
	c->buttons[OPGAMEPADBUTTON_DPAD_UP]    = buttonData[0];
	c->buttons[OPGAMEPADBUTTON_DPAD_DOWN]  = buttonData[1];
	// map buttons
	c->buttons[OPGAMEPADBUTTON_A] = buttonData[11];
	c->buttons[OPGAMEPADBUTTON_B] = buttonData[12];
	c->buttons[OPGAMEPADBUTTON_X] = buttonData[13];
	c->buttons[OPGAMEPADBUTTON_Y] = buttonData[14];
	c->buttons[OPGAMEPADBUTTON_LEFT_SHOULDER]  = buttonData[8];
	c->buttons[OPGAMEPADBUTTON_RIGHT_SHOULDER] = buttonData[9];
	c->buttons[OPGAMEPADBUTTON_BACK]        = buttonData[5];
	c->buttons[OPGAMEPADBUTTON_START]       = buttonData[4];
	c->buttons[OPGAMEPADBUTTON_LEFT_THUMB]  = buttonData[6];
	c->buttons[OPGAMEPADBUTTON_RIGHT_THUMB] = buttonData[7];
}
#endif
//-----------------------------------------------------------------------------
#ifdef OPIFEX_WINDOWS
void __OPwinUpdateGamePad(OPgamePad* controller){
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState((DWORD)controller->controllerIndex, &controllerState);

	// check to see if the controller is connected
	if(result == ERROR_SUCCESS){
		// if it's connected, get the state
		controller->connected = true;

		// check other states
#pragma region Trigger and stick states

		controller->axes[OPGAMEPADAXIS_L2] = controllerState.Gamepad.bLeftTrigger / 255.0f;
		controller->axes[OPGAMEPADAXIS_R2] = controllerState.Gamepad.bRightTrigger / 255.0f;

		controller->axes[OPGAMEPADAXIS_LS_X] = controllerState.Gamepad.sThumbLX / (OPfloat)SHRT_MAX;
		controller->axes[OPGAMEPADAXIS_LS_Y] = controllerState.Gamepad.sThumbLY / (OPfloat)SHRT_MAX;

		controller->axes[OPGAMEPADAXIS_RS_X] = controllerState.Gamepad.sThumbRX / (OPfloat)SHRT_MAX;
		controller->axes[OPGAMEPADAXIS_RS_Y] = controllerState.Gamepad.sThumbRY / (OPfloat)SHRT_MAX;
#pragma endregion

#pragma region Button states

		controller->buttons[OPGAMEPADBUTTON_LEFT_SHOULDER] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;

		controller->buttons[OPGAMEPADBUTTON_RIGHT_SHOULDER] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

		controller->buttons[OPGAMEPADBUTTON_A] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A);

		controller->buttons[OPGAMEPADBUTTON_B] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B;

		controller->buttons[OPGAMEPADBUTTON_X] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X;

		controller->buttons[OPGAMEPADBUTTON_Y] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

		controller->buttons[OPGAMEPADBUTTON_DPAD_UP] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;

		controller->buttons[OPGAMEPADBUTTON_DPAD_DOWN] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;

		controller->buttons[OPGAMEPADBUTTON_DPAD_LEFT] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;

		controller->buttons[OPGAMEPADBUTTON_DPAD_RIGHT] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

		controller->buttons[OPGAMEPADBUTTON_START] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START;

		controller->buttons[OPGAMEPADBUTTON_BACK] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;

		controller->buttons[OPGAMEPADBUTTON_LEFT_THUMB] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;

		controller->buttons[OPGAMEPADBUTTON_RIGHT_THUMB] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
#pragma endregion

	}
	else {
		controller->connected = false;
	}
}
#endif

void OPgamePad::Update(){
	if(connected) {
		OPmemcpy(
			&prevButtons,
			&buttons,
			sizeof(bool) * _OPGAMEPADBUTTON_MAX);
		OPmemcpy(
			&prevAxes,
			&axes,
			sizeof(OPfloat) * _OPGAMEPADAXIS_MAX);
	}

#ifdef OPIFEX_ANDROID
	__OPandUpdateGamePad(controller);
#endif

#if defined(OPIFEX_UNIX) && !defined(OPIFEX_IOS)
	__OPlnxUpdateGamePad(this);
#endif

#ifdef OPIFEX_WINDOWS
	__OPwinUpdateGamePad(this);
#endif

	OPvec2 axis;
	OPfloat len;

	axis.x = axes[0];
	axis.y = axes[1];
	len = OPvec2Len(axis);
	axes[0] *= len > deadzone;
	axes[1] *= len > deadzone;

	axis.x = axes[2];
	axis.y = axes[3];
	len = OPvec2Len(axis);
	axes[2] *= len > deadzone;
	axes[3] *= len > deadzone;

	axis.x = axes[4];
	axis.y = axes[5];
	len = OPvec2Len(axis);
	axes[4] *= len > deadzone;
	axes[5] *= len > deadzone;
}

void OPgamePad::Reset(){
	OPbzero(&buttons, sizeof(i32) * _OPGAMEPADBUTTON_MAX);
	OPbzero(&prevButtons, sizeof(i32) * _OPGAMEPADBUTTON_MAX);
	OPbzero(&axes, sizeof(OPfloat) * _OPGAMEPADAXIS_MAX);
}

bool OPgamePad::AnyPrevInputIsDown() {
	ui32 count = _OPGAMEPADBUTTON_MAX;
	for (; count > 0; count--) {
		if (prevButtons[count]) return true;
	}
	return false;
}	

bool OPgamePad::AnyInputIsDown() {
	ui32 count = _OPGAMEPADBUTTON_MAX;
	for (; count > 0; count--) {
		if (buttons[count]) return true;
	}
	return false;
}


OPgamePad* OPgamePadSystem::Get(OPgamePadIndex index) {
	return &gamePads[index];
}

void OPgamePadSystem::Update()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		gamePads[i].Update();
	}
}

void OPgamePadSystem::Reset()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		gamePads[i].Reset();
	}
}

void OPgamePadSystem::SetDeadzones(OPfloat deadzone)
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		gamePads[i].SetDeadzone(deadzone);
	}
}

OPvec2 OPgamePad::LeftThumb() {
	return OPvec2(axes[OPGAMEPADAXIS_LS_X], axes[OPGAMEPADAXIS_LS_Y]);
}

OPvec2 OPgamePad::RightThumb() {
	return OPvec2(axes[OPGAMEPADAXIS_RS_X], axes[OPGAMEPADAXIS_RS_Y]);
}
