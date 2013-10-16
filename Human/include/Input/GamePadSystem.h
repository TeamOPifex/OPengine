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
	OPfloat		prevAxes[GamePadAxes_Max];
} GamePadController;

extern GamePadController GamePadControllers[CONTROLLERS];

GamePadController* OPgamePadController(GamePadIndex index);

void OPgamePadUpdate(GamePadController* controller);
void OPgamePadReset(GamePadController* controller);
void OPgamePadSetDeadzone(GamePadController* controller, OPfloat deadzone);

void OPgamePadSystemUpdate();
void OPgamePadSystemReset();
void OPgamePadSetDeadzones(OPfloat deadzone);

OPvec2 OPgamePadLeftThumb(GamePadController* controller);
OPvec2 OPgamePadRightThumb(GamePadController* controller);

inline bool OPgamePadIsDown(GamePadController* controller, GamePadButton button){
	return controller->buttons[button];
}

inline bool OPgamePadIsUp(GamePadController* controller, GamePadButton button){
	return !controller->buttons[button];
}

inline bool OPgamePadWasPressed(GamePadController* controller, GamePadButton button){
	return controller->buttons[button] && !controller->prevButtons[button];
}

inline bool OPgamePadWasReleased(GamePadController* controller, GamePadButton button){
	return !controller->buttons[button] && controller->prevButtons[button];
}

inline bool OPgamePadLeftThumbIsRight(GamePadController* controller) { return controller->axes[LS_X] > 0.0f; }
inline bool OPgamePadLeftThumbNowRight(GamePadController* controller) { return controller->prevAxes[LS_X] <= 0.0f && controller->axes[LS_X] > 0.0f; }
inline bool OPgamePadLeftThumbWasRight(GamePadController* controller) { return controller->prevAxes[LS_X] > 0.0f && controller->axes[LS_X] <= 0.0f; }
inline bool OPgamePadLeftThumbIsLeft(GamePadController* controller) { return controller->axes[LS_X] < 0.0f; }
inline bool OPgamePadLeftThumbNowLeft(GamePadController* controller) { return controller->prevAxes[LS_X] >= 0.0f && controller->axes[LS_X] < 0.0f; }
inline bool OPgamePadLeftThumbWasLeft(GamePadController* controller) { return controller->prevAxes[LS_X] < 0.0f && controller->axes[LS_X] >= 0.0f; }
inline OPfloat OPgamePadLeftThumbX(GamePadController* controller) {
	return controller->axes[LS_X];
}

inline bool OPgamePadLeftThumbIsUp(GamePadController* controller) { return controller->axes[LS_Y] > 0.0f; }
inline bool OPgamePadLeftThumbNowUp(GamePadController* controller) { return controller->prevAxes[LS_Y] <= 0.0f && controller->axes[LS_Y] > 0.0f; }
inline bool OPgamePadLeftThumbWasUp(GamePadController* controller) { return controller->prevAxes[LS_Y] > 0.0f && controller->axes[LS_Y] <= 0.0f; }
inline bool OPgamePadLeftThumbIsDown(GamePadController* controller) { return controller->axes[LS_Y] < 0.0f; }
inline bool OPgamePadLeftThumbNowDown(GamePadController* controller) { return controller->prevAxes[LS_Y] >= 0.0f && controller->axes[LS_Y] < 0.0f; }
inline bool OPgamePadLeftThumbWasDown(GamePadController* controller) { return controller->prevAxes[LS_Y] < 0.0f && controller->axes[LS_Y] >= 0.0f; }
inline OPfloat OPgamePadLeftThumbY(GamePadController* controller) {
	return controller->axes[LS_Y];
}

inline bool OPgamePadRightThumbIsRight(GamePadController* controller) { return controller->axes[RS_X] > 0.0f; }
inline bool OPgamePadRightThumbNowRight(GamePadController* controller) { return controller->prevAxes[RS_X] <= 0.0f && controller->axes[RS_X] > 0.0f; }
inline bool OPgamePadRightThumbWasRight(GamePadController* controller) { return controller->prevAxes[RS_X] > 0.0f && controller->axes[RS_X] <= 0.0f; }
inline bool OPgamePadRightThumbIsLeft(GamePadController* controller) { return controller->axes[RS_X] < 0.0f; }
inline bool OPgamePadRightThumbNowLeft(GamePadController* controller) { return controller->prevAxes[RS_X] >= 0.0f && controller->axes[RS_X] < 0.0f; }
inline bool OPgamePadRightThumbWasLeft(GamePadController* controller) { return controller->prevAxes[RS_X] < 0.0f && controller->axes[RS_X] >= 0.0f; }
inline OPfloat OPgamePadRightThumbX(GamePadController* controller) {
	return controller->axes[RS_X];
}


inline bool OPgamePadRightThumbIsUp(GamePadController* controller) { return controller->axes[RS_Y] > 0.0f; }
inline bool OPgamePadRightThumbNowUp(GamePadController* controller) { return controller->prevAxes[RS_Y] <= 0.0f && controller->axes[LS_Y] > 0.0f; }
inline bool OPgamePadRightThumbWasUp(GamePadController* controller) { return controller->prevAxes[RS_Y] > 0.0f && controller->axes[LS_Y] <= 0.0f; }
inline bool OPgamePadRightThumbIsDown(GamePadController* controller) { return controller->axes[RS_Y] < 0.0f; }
inline bool OPgamePadRightThumbNowDown(GamePadController* controller) { return controller->prevAxes[RS_Y] >= 0.0f && controller->axes[LS_Y] < 0.0f; }
inline bool OPgamePadRightThumbWasDown(GamePadController* controller) { return controller->prevAxes[RS_Y] < 0.0f && controller->axes[LS_Y] >= 0.0f; }
inline OPfloat OPgamePadRightThumbY(GamePadController* controller) {
	return controller->axes[RS_Y];
}

inline OPfloat OPgamePadLeftTrigger(GamePadController* controller) {
	return controller->axes[L2];
}

inline OPfloat OPgamePadRightTrigger(GamePadController* controller) {
	return controller->axes[R2];
}

inline bool OPgamePadIsConnected(GamePadController* controller) {
	return controller->connected;
}

inline void OPgamePadSetDeadzone(GamePadController* controller, OPfloat deadzone) {
	controller->deadzone = deadzone;
}
