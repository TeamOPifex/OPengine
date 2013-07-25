#pragma once

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)

#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#endif

#include "GamePadState.h"

#define CONTROLLERS 4

class GamePadSystem {
public:
	static void Update();
	static GamePadState* Controller(GamePadIndex index);
private:
	static GamePadState _gamePadStates[CONTROLLERS];
};