#include "./Human/include/Input/GamePadSystem.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	// TODO - Linux GamePad Support
#elif defined(OPIFEX_WINDOWS)
	#include <Windows.h>
	#include <Xinput.h>
	#pragma comment(lib, "XInput.lib")
#elif defined(OPIFEX_ANDROID)
	#include "./Core/include/Core.h"
	#include "./Human/include/Utilities/AndroidNDK.h"
#endif

OPgamePadController GamePadControllers[CONTROLLERS] = {
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

OPgamePadController* OPgamePad(GamePadIndex index) {
	return &GamePadControllers[index];
}

#ifdef OPIFEX_ANDROID

	#define CONTROLLER_CLASS_NAME "tv/ouya/console/api/OuyaController"

	jobject OPjniGetControllerByPlayer( int playerNum) {
		OPJniMethodInfo methodInfo;
		if (!OPjniGetStaticMethodInfo(methodInfo,
			CONTROLLER_CLASS_NAME, "getControllerByPlayer",
			"(I)Ltv/ouya/console/api/OuyaController;")) {
				return 0;
		}
		return methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
			methodInfo.methodID, playerNum);
	}


	bool OPjniGetControllerButton(jobject controller, int button) {

		OPJniMethodInfo methodInfo2;
		if (!OPjniGetMethodInfo(methodInfo2,
			CONTROLLER_CLASS_NAME, "getButton", "(I)Z")) {
				OPlog("Class method not found");
				return false;
		}
		
		JNIEnvironment()->DeleteLocalRef(methodInfo2.classID);

		bool result = JNIEnvironment()->CallBooleanMethod(controller, methodInfo2.methodID, button);

		if(result) {
			OPlog("Button Pressed: %d", button);
		} else {

		}

		return result;
	}

	float OPjniGetAxisValue( jobject controller, int ouyaAxis ) {

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


void OPgamePadUpdate(OPgamePadController* controller){
	if(OPgamePadIsConnected(controller)) {
		OPmemcpy(
			&controller->prevButtons, 
			&controller->buttons, 
			sizeof(bool) * GamePadButton_Max);
		OPmemcpy(
			&controller->prevAxes, 
			&controller->axes, 
			sizeof(OPfloat) * GamePadAxes_Max);
	}

#ifdef OPIFEX_ANDROID

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

#endif

#ifdef OPIFEX_WIN32
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

		controller->axes[LS_X] = controllerState.Gamepad.sThumbLX / (float)SHRT_MAX;
		controller->axes[LS_Y] = controllerState.Gamepad.sThumbLY / (float)SHRT_MAX;

		controller->axes[RS_X] = controllerState.Gamepad.sThumbRX / (float)SHRT_MAX;
		controller->axes[RS_Y] = controllerState.Gamepad.sThumbRY / (float)SHRT_MAX;
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
#endif
	
		
	for(ui32 i = 0; i < GamePadAxes_Max; i++) {
		if(OPabs(controller->axes[i]) < controller->deadzone) {
			controller->axes[i] = 0;
		}
	}
}

void OPgamePadReset(OPgamePadController* controller){
	OPbzero(&controller->buttons, sizeof(i32) * GamePadButton_Max);
	OPbzero(&controller->prevButtons, sizeof(i32) * GamePadButton_Max);
	OPbzero(&controller->axes, sizeof(OPfloat) * GamePadAxes_Max);
}


void OPgamePadSystemUpdate()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadUpdate( OPgamePad( (GamePadIndex)i ) );
	}
}

void OPgamePadSystemReset()
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadReset( OPgamePad((GamePadIndex)i));
	}
}

void OPgamePadSetDeadzones(OPfloat deadzone)
{
	for ( OPint i = CONTROLLERS; i--; )
	{
		OPgamePadSetDeadzone( OPgamePad((GamePadIndex)i), deadzone);
	}
}

OPvec2 OPgamePadLeftThumb(OPgamePadController* controller) {
	OPvec2 tmp;
	tmp.x = controller->axes[LS_X];
	tmp.y = controller->axes[LS_Y];
	return tmp;
}
OPvec2 OPgamePadRightThumb(OPgamePadController* controller) {
	OPvec2 tmp;
	tmp.x = controller->axes[RS_X];
	tmp.y = controller->axes[RS_Y];
	return tmp;
}