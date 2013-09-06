#include "./Human/include/Input/GamePadSystem.h"
#include "./Core/include/Log.h"

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <Windows.h>
#include <Xinput.h>
#endif

GamePadState GamePadSystem::_gamePadStates[CONTROLLERS];

GamePadState* GamePadSystem::Controller(GamePadIndex index){
	return &_gamePadStates[index];
}
#ifdef OPIFEX_ANDROID

#include "./Human/include/Utilities/AndroidNDK.h"

#define CONTROLLER_CLASS_NAME "tv/ouya/console/api/OuyaController"

jobject GamePadSystem::getControllerByPlayer( int playerNum) {
	OPLog("Get Player");
    JniMethodInfo methodInfo;
    if (!getStaticMethodInfo(methodInfo,
            CONTROLLER_CLASS_NAME, "getControllerByPlayer",
            "(I)Ltv/ouya/console/api/OuyaController;")) {
        return 0;
    }
    return methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
            methodInfo.methodID, playerNum);
}


bool GamePadSystem::getControllerButton(jobject controller, int button) {

	JniMethodInfo methodInfo2;
    if (!getMethodInfo(methodInfo2,
            CONTROLLER_CLASS_NAME, "getButton", "(I)Z")) {
    	OPLog("Class method not found");
        return false;
    }

    OPLog("Class Method was Found");
    JNIEnvironment()->DeleteLocalRef(methodInfo2.classID);
    OPLog("Local Reference Deleted");

    bool result = JNIEnvironment()->CallBooleanMethod(controller, methodInfo2.methodID, button);

    if(result) {
    	OPLog("Button Pressed");
    } else {
    	OPLog("Not Pressed");
    }

    return result;
}

float GamePadSystem::getAxisValue( jobject controller, int ouyaAxis ) {
    
    JniMethodInfo methodInfo;

    if (!getMethodInfo(methodInfo,
            CONTROLLER_CLASS_NAME, "getAxisValue", "(I)F")) {
        return 0;
    }

    jfloat f = methodInfo.env->CallFloatMethod(controller, methodInfo.methodID, ouyaAxis);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);

    return f;
}

#endif

void GamePadSystem::Update(){
		for(int i = 0; i < CONTROLLERS; i++){
			GamePadState* gps = &_gamePadStates[i];
			if(gps->IsConnected()) {
				gps->Update();
			}

#ifdef OPIFEX_ANDROID
			jobject controller = getControllerByPlayer(i);
			if(controller == 0) {
				return;
			}

			gps->SetConnected(true);

			gps->SetButton(GamePad_Button_A,
					getControllerButton(controller, OUYA_BUTTON_O));
			gps->SetButton(GamePad_Button_X,
					getControllerButton(controller, OUYA_BUTTON_U));
			gps->SetButton(GamePad_Button_B,
					getControllerButton(controller, OUYA_BUTTON_A));
			gps->SetButton(GamePad_Button_Y,
					getControllerButton(controller, OUYA_BUTTON_Y));


			gps->SetButton(GamePad_Button_LEFT_SHOULDER,
					getControllerButton(controller, OUYA_BUTTON_L1));
			gps->SetButton(GamePad_Button_RIGHT_SHOULDER,
					getControllerButton(controller, OUYA_BUTTON_R1));

			gps->SetButton(GamePad_Button_LEFT_THUMB,
					getControllerButton(controller, OUYA_BUTTON_L3));
			gps->SetButton(GamePad_Button_RIGHT_THUMB,
					getControllerButton(controller, OUYA_BUTTON_R3));

			gps->SetButton(GamePad_Button_DPAD_UP,
					getControllerButton(controller, OUYA_BUTTON_DPAD_UP));
			gps->SetButton(GamePad_Button_DPAD_DOWN,
					getControllerButton(controller, OUYA_BUTTON_DPAD_DOWN));
			gps->SetButton(GamePad_Button_DPAD_LEFT,
					getControllerButton(controller, OUYA_BUTTON_DPAD_LEFT));
			gps->SetButton(GamePad_Button_DPAD_RIGHT,
					getControllerButton(controller, OUYA_BUTTON_DPAD_RIGHT));

			gps->SetButton(GamePad_Button_START,
					getControllerButton(controller, OUYA_BUTTON_MENU));


			gps->SetLeftTrigger(getAxisValue(controller, OUYA_AXIS_L2));
			gps->SetRightTrigger(getAxisValue(controller, OUYA_AXIS_R2));

			gps->SetLeftThumb(
				getAxisValue(controller, OUYA_AXIS_LS_X),
				getAxisValue(controller, OUYA_AXIS_LS_Y)
			);

			gps->SetRightThumb(
				getAxisValue(controller, OUYA_AXIS_RS_X),
				getAxisValue(controller, OUYA_AXIS_RS_Y)
			);  
#endif
#ifdef OPIFEX_WIN32
			XINPUT_STATE controllerState;
			ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
			DWORD result = XInputGetState(i, &controllerState);

			// check to see if the controller is connected
			if(result == ERROR_SUCCESS){
				// if it's connected, get the state

				gps->SetConnected(true);

				// check other states
#pragma region Trigger and stick states
				gps->SetLeftTrigger(controllerState.Gamepad.bLeftTrigger / 255.0f);
				gps->SetRightTrigger(controllerState.Gamepad.bRightTrigger / 255.0f);

				gps->SetLeftThumb(
					controllerState.Gamepad.sThumbLX / (float)SHRT_MAX,
					controllerState.Gamepad.sThumbLY / (float)SHRT_MAX
				);

				gps->SetRightThumb(
					controllerState.Gamepad.sThumbRX / (float)SHRT_MAX,
					controllerState.Gamepad.sThumbRY / (float)SHRT_MAX
				);  
#pragma endregion


#pragma region Button states
				gps->SetButton(
					GamePad_Button_LEFT_SHOULDER,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER
					);
				gps->SetButton(
					GamePad_Button_RIGHT_SHOULDER,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER
					);

				gps->SetButton(
					GamePad_Button_A,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A
					);

				gps->SetButton(
					GamePad_Button_B,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B
					);

				gps->SetButton(
					GamePad_Button_X,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X
					);

				gps->SetButton(
					GamePad_Button_DPAD_UP,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP
					);

				gps->SetButton(
					GamePad_Button_DPAD_DOWN,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN
					);

				gps->SetButton(
					GamePad_Button_DPAD_LEFT,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT
					);

				gps->SetButton(
					GamePad_Button_DPAD_RIGHT,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT
					);

				gps->SetButton(
					GamePad_Button_START,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START
					);

				gps->SetButton(
					GamePad_Button_BACK,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK
					);

				gps->SetButton(
					GamePad_Button_LEFT_THUMB,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB
					);

				gps->SetButton(
					GamePad_Button_RIGHT_THUMB,
					controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB
					);  
#pragma endregion

			}
			else
				_gamePadStates[i].SetConnected(false);


			
#endif
		}
}