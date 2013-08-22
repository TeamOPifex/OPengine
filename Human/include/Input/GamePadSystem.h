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
	#ifdef OPIFEX_ANDROID
	static void Update(JNIEnv* env);
	#else
	static void Update();
	#endif
	static GamePadState* Controller(GamePadIndex index);
private:
	#ifdef OPIFEX_ANDROID
	static jobject getControllerByPlayer( JNIEnv* env, int playerNum );
	static bool getControllerButton( JNIEnv* env, jobject controller, int button );
	static float getAxisValue( JNIEnv* env, jobject controller, int ouyaAxis);
	#endif
	static GamePadState _gamePadStates[CONTROLLERS];
};