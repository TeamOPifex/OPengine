#pragma once

#include "./Core/include/Types.h"
#include "./Math/include/Vector2.h"
#include "GamePadEnums.h"


#define CONTROLLERS 4


typedef struct{
	OPint		playerIndex;
	bool		connected;
	OPfloat		deadzone;
	bool		buttons[GamePadButton_Max];
	bool		prevButtons[GamePadButton_Max];
	OPfloat		axes[GamePadAxes_Max];
} GamePadController;

extern GamePadController GamePadControllers[CONTROLLERS];

GamePadController* OPgamePadController(GamePadIndex index);

void OPgamePadUpdate(GamePadController* controller);
void OPgamePadReset(GamePadController* controller);
void OPgamePadSetDeadzone(GamePadController* controller, OPfloat deadzone);

void OPgamePadSystemUpdate();
void OPgamePadSystemReset();
void OPgamePadSetDeadzones(OPfloat deadzone);

bool OPgamePadIsDown(GamePadController* controller, GamePadButton button);
bool OPgamePadIsUp(GamePadController* controller, GamePadButton button);	
bool OPgamePadWasPressed(GamePadController* controller, GamePadButton button);
bool OPgamePadWasReleased(GamePadController* controller, GamePadButton button);
OPvec2 OPgamePadLeftThumb(GamePadController* controller);
OPfloat OPgamePadLeftThumbX(GamePadController* controller);
OPfloat OPgamePadLeftThumbY(GamePadController* controller);
OPfloat OPgamePadRightThumbX(GamePadController* controller);
OPfloat OPgamePadRightThumbY(GamePadController* controller);
OPfloat OPgamePadLeftTrigger(GamePadController* controller);
OPfloat OPgamePadRightTrigger(GamePadController* controller);
bool OPgamePadIsConnected(GamePadController* controller);