#include "GamePadSystem.h"

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
//#include <Windows.h>
//#include <Xinput.h>
#endif

GamePadState GamePadSystem::_gamePadStates[CONTROLLERS];

GamePadState* GamePadSystem::Controller(GamePadIndex index){
	return &_gamePadStates[index];
}

void GamePadSystem::Update(){

		for(int i = 0; i < CONTROLLERS; i++){
#ifdef OPIFEX_ANDROID
			_gamePadStates[i].Update();
#endif
#ifdef OPIFEX_WIN32
			XINPUT_STATE controllerState;
			ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
			DWORD result = XInputGetState(i, &controllerState);
			GamePadState* gps = &_gamePadStates[i];

			// check to see if the controller is connected
			if(result == ERROR_SUCCESS){
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
				_gamePadStates[i].SetConnected(true);
			
#endif
		}
}