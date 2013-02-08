#pragma once

#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#include "GamePadState.h"

#define CONTROLLERS 4

class GamePadSystem {
public:
	static void Update();
	static GamePadState* Controller(GamePadIndex index);
private:
	static GamePadState _gamePadStates[CONTROLLERS];
};