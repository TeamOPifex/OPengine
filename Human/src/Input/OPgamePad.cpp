#include "./Human/include/Input/OPgamePad.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
	
#if !defined(OPIFEX_ANDROID) && defined(OPIFEX_UNIX)
	#include <GLFW/glfw3.h>
#elif defined(OPIFEX_WINDOWS)
	#include <Windows.h>
	#include <Xinput.h>
	#pragma comment(lib, "XInput.lib")
#elif defined(OPIFEX_ANDROID)
	#include "./Core/include/Core.h"
	#include "./Human/include/Utilities/AndroidNDK.h"
#endif

OPgamePad GamePads[CONTROLLERS] = {
	{
		0,
		false,
		0.1f
	},
	{
		1,
		false,
		0.1f
	},
	{
		2,
		false,
		0.1f
	},
	{
		3,
		false,
		0.1f
	}
}; 

//-----------------------------------------------------------------------------
OPgamePad* OPgamePadIndex(OPgamePadIndices index) {
	return &GamePads[index];
}
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

	controller->buttons[GamePad_Button_A] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_O);
	controller->buttons[GamePad_Button_X] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_U);
	controller->buttons[GamePad_Button_B] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_A);
	controller->buttons[GamePad_Button_Y] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_Y);

	controller->buttons[GamePad_Button_LEFT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L1);
	controller->buttons[GamePad_Button_RIGHT_SHOULDER] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R1);

	controller->buttons[GamePad_Button_LEFT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_L3);
	controller->buttons[GamePad_Button_RIGHT_THUMB] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_R3);


	controller->buttons[GamePad_Button_DPAD_UP] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_UP);
	controller->buttons[GamePad_Button_DPAD_DOWN] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_DOWN);
	controller->buttons[GamePad_Button_DPAD_LEFT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_LEFT);
	controller->buttons[GamePad_Button_DPAD_RIGHT] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_DPAD_RIGHT);

	controller->buttons[GamePad_Button_START] = OPjniGetControllerButton(jcontroller, OUYA_BUTTON_MENU);

	controller->axes[L2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_L2);
	controller->axes[R2] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_R2);

	controller->axes[LS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_X);
	controller->axes[LS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_LS_Y);

	controller->axes[RS_X] = OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_X);
	controller->axes[RS_Y] = -OPjniGetAxisValue(jcontroller, OUYA_AXIS_RS_Y);
}
#endif
//-----------------------------------------------------------------------------
#if !defined(OPIFEX_ANDROID) && defined(OPIFEX_UNIX)
void __OPlnxUpdateGamePad(OPgamePad* c){
	i32  axes = 0, buttons = 0;
	const f32* axisData   = NULL;
	const ui8* buttonData = NULL;

	// make sure the gamepad is connected
	if(!glfwJoystickPresent((i32)c->playerIndex)){
		c->connected = 0;
		return;
	}

	// get axis data, andmake sure the number of axes are expected
	axisData = glfwGetJoystickAxes(c->playerIndex, &axes); 
	if(axes != 8){
		// game pads should have 8 axes
		// 2 for each stick, and 2 for the dpad
		c->connected = 0;
		return;
	}
	
	// map left stick and trigger
	c->axes[LS_X] =  axisData[0];
	c->axes[LS_Y] = -axisData[1];
	c->axes[L2]   =  (axisData[2] + 1.0f) / 2.0f;

	// map right stick and trigger
	c->axes[RS_X] =  axisData[3];
	c->axes[RS_Y] = -axisData[4];
	c->axes[R2]   =  (axisData[5] + 1.0f) / 2.0f;

	// map the dpad
	c->buttons[GamePad_Button_DPAD_LEFT]  = axisData[6] == -1.0f;
	c->buttons[GamePad_Button_DPAD_RIGHT] = axisData[6] ==  1.0f;
	c->buttons[GamePad_Button_DPAD_UP]    = axisData[7] == -1.0f;
	c->buttons[GamePad_Button_DPAD_DOWN]  = axisData[7] ==  1.0f;

	// get button data, make sure it's all kosher
	buttonData = glfwGetJoystickButtons(c->playerIndex, &buttons);
	if(buttons != 11){
		// game pads should have 10 buttons
		c->connected = 0;
		return;
	}

	// map buttons
	c->buttons[GamePad_Button_A] = buttonData[0];
	c->buttons[GamePad_Button_B] = buttonData[1];
	c->buttons[GamePad_Button_X] = buttonData[2];
	c->buttons[GamePad_Button_Y] = buttonData[3];
	c->buttons[GamePad_Button_LEFT_SHOULDER]  = buttonData[4];
	c->buttons[GamePad_Button_RIGHT_SHOULDER] = buttonData[5];
	c->buttons[GamePad_Button_BACK]        = buttonData[6];
	c->buttons[GamePad_Button_START]       = buttonData[7];
	//c->buttons[GamePad_Button_A]         = buttonData[8];
	c->buttons[GamePad_Button_LEFT_THUMB]  = buttonData[9];
	c->buttons[GamePad_Button_RIGHT_THUMB] = buttonData[10];
}
#endif
//-----------------------------------------------------------------------------
#ifdef OPIFEX_WINDOWS
void __OPwinUpdateGamePad(OPgamePad* controller){
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(controller->playerIndex, &controllerState);

	// check to see if the controller is connected
	if(result == ERROR_SUCCESS){
		// if it's connected, get the state
		controller->connected = true;

		// check other states
#pragma region Trigger and stick states

		controller->axes[L2] = controllerState.Gamepad.bLeftTrigger / 255.0f;
		controller->axes[R2] = controllerState.Gamepad.bRightTrigger / 255.0f;

		controller->axes[LS_X] = controllerState.Gamepad.sThumbLX / (OPfloat)SHRT_MAX;
		controller->axes[LS_Y] = controllerState.Gamepad.sThumbLY / (OPfloat)SHRT_MAX;

		controller->axes[RS_X] = controllerState.Gamepad.sThumbRX / (OPfloat)SHRT_MAX;
		controller->axes[RS_Y] = controllerState.Gamepad.sThumbRY / (OPfloat)SHRT_MAX;
#pragma endregion
		
#pragma region Button states

		controller->buttons[GamePad_Button_LEFT_SHOULDER] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;

		controller->buttons[GamePad_Button_RIGHT_SHOULDER] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

		controller->buttons[GamePad_Button_A] =
			(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A);

		controller->buttons[GamePad_Button_B] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B;

		controller->buttons[GamePad_Button_X] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X;

		controller->buttons[GamePad_Button_Y] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

		controller->buttons[GamePad_Button_DPAD_UP] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;

		controller->buttons[GamePad_Button_DPAD_DOWN] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;

		controller->buttons[GamePad_Button_DPAD_LEFT] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;

		controller->buttons[GamePad_Button_DPAD_RIGHT] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

		controller->buttons[GamePad_Button_START] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START;

		controller->buttons[GamePad_Button_BACK] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;

		controller->buttons[GamePad_Button_LEFT_THUMB] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;

		controller->buttons[GamePad_Button_RIGHT_THUMB] =
			controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
#pragma endregion

	}
	else {
		controller->connected = false;
	}
}
#endif

void OPgamePadUpdate(OPgamePad* controller){
	if(OPgamePadIsConnected(controller)) {
		OPmemcpy(
			&controller->prevButtons, 
			&controller->buttons, 
			sizeof(OPint) * GamePadButton_Max);
		OPmemcpy(
			&controller->prevAxes, 
			&controller->axes, 
			sizeof(OPfloat) * GamePadAxes_Max);
	}

#ifdef OPIFEX_ANDROID
	__OPandUpdateGamePad(controller);
#endif
	
#if !defined(OPIFEX_ANDROID) && defined(OPIFEX_UNIX)
	__OPlnxUpdateGamePad(controller);	
#endif

#ifdef OPIFEX_WINDOWS
	__OPwinUpdateGamePad(controller);
#endif
	
	OPvec2 axis;
	OPfloat len;

	axis.x = controller->axes[0];
	axis.y = controller->axes[1];
	OPvec2len(&len, &axis);
	controller->axes[0] *= len > controller->deadzone;
	controller->axes[1] *= len > controller->deadzone;
	
	axis.x = controller->axes[2];
	axis.y = controller->axes[3];
	OPvec2len(&len, &axis);
	controller->axes[2] *= len > controller->deadzone;
	controller->axes[3] *= len > controller->deadzone;

	axis.x = controller->axes[4];
	axis.y = controller->axes[5];
	OPvec2len(&len, &axis);
	controller->axes[4] *= len > controller->deadzone;
	controller->axes[5] *= len > controller->deadzone;
}

void OPgamePadReset(OPgamePad* controller){
	OPbzero(&controller->buttons, sizeof(i32) * GamePadButton_Max);
	OPbzero(&controller->prevButtons, sizeof(i32) * GamePadButton_Max);
	OPbzero(&controller->axes, sizeof(OPfloat) * GamePadAxes_Max);
}


void OPgamePadSystemUpdate()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadUpdate( OPgamePadIndex( (OPgamePadIndices)i ) );
	}
}

void OPgamePadSystemReset()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadReset( OPgamePadIndex((OPgamePadIndices)i));
	}
}

void OPgamePadSetDeadzones(OPfloat deadzone)
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadSetDeadzone( OPgamePadIndex((OPgamePadIndices)i), deadzone);
	}
}

OPvec2 OPgamePadLeftThumb(OPgamePad* controller) {
	OPvec2 tmp;
	tmp.x = controller->axes[LS_X];
	tmp.y = controller->axes[LS_Y];
	return tmp;
}
OPvec2 OPgamePadRightThumb(OPgamePad* controller) {
	OPvec2 tmp;
	tmp.x = controller->axes[RS_X];
	tmp.y = controller->axes[RS_Y];
	return tmp;
}
