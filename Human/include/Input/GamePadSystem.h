#pragma once

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)

#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

#elif defined(OPIFEX_ANDROID)
	#include <jni.h>
#endif

#include "GamePadState.h"

#define CONTROLLERS 4

class GamePadSystem {
public:
	static void Update();
	static GamePadState* Controller(GamePadIndex index);
private:
	#ifdef OPIFEX_ANDROID
		static jobject getControllerByPlayer( int playerNum );
		static bool getControllerButton( jobject controller, int button );
		static float getAxisValue( jobject controller, int ouyaAxis);
	#endif
	static GamePadState _gamePadStates[CONTROLLERS];
};