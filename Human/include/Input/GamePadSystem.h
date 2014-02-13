#ifndef OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM
#define OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM

#include "./Core/include/Types.h"
#include "./Math/include/Vector2.h"
#include "GamePadEnums.h"

#define CONTROLLERS 4

typedef struct {
	OPint		playerIndex;
	bool		connected;
	OPfloat		deadzone;
	bool		buttons[GamePadButton_Max];
	bool		prevButtons[GamePadButton_Max];
	OPfloat		axes[GamePadAxes_Max];
	OPfloat		prevAxes[GamePadAxes_Max];
} OPgamePadController;

extern OPgamePadController GamePadControllers[CONTROLLERS];

OPgamePadController* OPgamePad(GamePadIndex index);

void OPgamePadUpdate(OPgamePadController* controller);
void OPgamePadReset(OPgamePadController* controller);
void OPgamePadSetDeadzone(OPgamePadController* controller, OPfloat deadzone);

void OPgamePadSystemUpdate();
void OPgamePadSystemReset();
void OPgamePadSetDeadzones(OPfloat deadzone);

OPvec2 OPgamePadLeftThumb(OPgamePadController* controller);
OPvec2 OPgamePadRightThumb(OPgamePadController* controller);

inline bool OPgamePadIsDown(OPgamePadController* controller, GamePadButton button){
	return controller->buttons[button];
}

inline bool OPgamePadIsUp(OPgamePadController* controller, GamePadButton button){
	return !controller->buttons[button];
}

inline bool OPgamePadWasPressed(OPgamePadController* controller, GamePadButton button){
	return controller->buttons[button] && !controller->prevButtons[button];
}

inline bool OPgamePadWasReleased(OPgamePadController* controller, GamePadButton button){
	return !controller->buttons[button] && controller->prevButtons[button];
}

inline bool OPgamePadLeftThumbIsRight(OPgamePadController* controller) { return controller->axes[LS_X] > controller->deadzone; }
inline bool OPgamePadLeftThumbNowRight(OPgamePadController* controller) { return controller->prevAxes[LS_X] <= controller->deadzone && controller->axes[LS_X] > controller->deadzone; }
inline bool OPgamePadLeftThumbWasRight(OPgamePadController* controller) { return controller->prevAxes[LS_X] > controller->deadzone && controller->axes[LS_X] <= controller->deadzone; }
inline bool OPgamePadLeftThumbIsLeft(OPgamePadController* controller) { return controller->axes[LS_X] < -controller->deadzone; }
inline bool OPgamePadLeftThumbNowLeft(OPgamePadController* controller) { return controller->prevAxes[LS_X] >= -controller->deadzone && controller->axes[LS_X] < -controller->deadzone; }
inline bool OPgamePadLeftThumbWasLeft(OPgamePadController* controller) { return controller->prevAxes[LS_X] < -controller->deadzone && controller->axes[LS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbX(OPgamePadController* controller) {
	return controller->axes[LS_X];
}

inline bool OPgamePadLeftThumbIsUp(OPgamePadController* controller) { return controller->axes[LS_Y] > controller->deadzone; }
inline bool OPgamePadLeftThumbNowUp(OPgamePadController* controller) { return controller->prevAxes[LS_Y] <= controller->deadzone && controller->axes[LS_Y] > controller->deadzone; }
inline bool OPgamePadLeftThumbWasUp(OPgamePadController* controller) { return controller->prevAxes[LS_Y] > controller->deadzone && controller->axes[LS_Y] <= controller->deadzone; }
inline bool OPgamePadLeftThumbIsDown(OPgamePadController* controller) { return controller->axes[LS_Y] < -controller->deadzone; }
inline bool OPgamePadLeftThumbNowDown(OPgamePadController* controller) { return controller->prevAxes[LS_Y] >= -controller->deadzone && controller->axes[LS_Y] < -controller->deadzone; }
inline bool OPgamePadLeftThumbWasDown(OPgamePadController* controller) { return controller->prevAxes[LS_Y] < -controller->deadzone && controller->axes[LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbY(OPgamePadController* controller) {
	return controller->axes[LS_Y];
}

inline bool OPgamePadRightThumbIsRight(OPgamePadController* controller) { return controller->axes[RS_X] > controller->deadzone; }
inline bool OPgamePadRightThumbNowRight(OPgamePadController* controller) { return controller->prevAxes[RS_X] <= controller->deadzone && controller->axes[RS_X] > controller->deadzone; }
inline bool OPgamePadRightThumbWasRight(OPgamePadController* controller) { return controller->prevAxes[RS_X] > controller->deadzone && controller->axes[RS_X] <= controller->deadzone; }
inline bool OPgamePadRightThumbIsLeft(OPgamePadController* controller) { return controller->axes[RS_X] < -controller->deadzone; }
inline bool OPgamePadRightThumbNowLeft(OPgamePadController* controller) { return controller->prevAxes[RS_X] >= -controller->deadzone && controller->axes[RS_X] < -controller->deadzone; }
inline bool OPgamePadRightThumbWasLeft(OPgamePadController* controller) { return controller->prevAxes[RS_X] < -controller->deadzone && controller->axes[RS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbX(OPgamePadController* controller) {
	return controller->axes[RS_X];
}


inline bool OPgamePadRightThumbIsUp(OPgamePadController* controller) { return controller->axes[RS_Y] > controller->deadzone; }
inline bool OPgamePadRightThumbNowUp(OPgamePadController* controller) { return controller->prevAxes[RS_Y] <= controller->deadzone && controller->axes[LS_Y] > controller->deadzone; }
inline bool OPgamePadRightThumbWasUp(OPgamePadController* controller) { return controller->prevAxes[RS_Y] > controller->deadzone && controller->axes[LS_Y] <= controller->deadzone; }
inline bool OPgamePadRightThumbIsDown(OPgamePadController* controller) { return controller->axes[RS_Y] < -controller->deadzone; }
inline bool OPgamePadRightThumbNowDown(OPgamePadController* controller) { return controller->prevAxes[RS_Y] >= -controller->deadzone && controller->axes[LS_Y] < -controller->deadzone; }
inline bool OPgamePadRightThumbWasDown(OPgamePadController* controller) { return controller->prevAxes[RS_Y] < -controller->deadzone && controller->axes[LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbY(OPgamePadController* controller) {
	return controller->axes[RS_Y];
}

inline OPfloat OPgamePadLeftTrigger(OPgamePadController* controller) {
	return controller->axes[L2];
}
inline bool OPgamePadLeftTriggerWasPressed(OPgamePadController* controller) { return controller->axes[L2] > 0 && controller->prevAxes[L2] <= 0; }
inline bool OPgamePadLeftTriggerWasReleased(OPgamePadController* controller) { return controller->axes[L2] <= 0 && controller->prevAxes[L2] > 0; }
inline bool OPgamePadLeftTriggerIsDown(OPgamePadController* controller) { return controller->axes[L2] > 0; }
inline bool OPgamePadLeftTriggerIsUp(OPgamePadController* controller) { return controller->axes[L2] <= 0; }

inline OPfloat OPgamePadRightTrigger(OPgamePadController* controller) {
	return controller->axes[R2];
}
inline bool OPgamePadRightTriggerWasPressed(OPgamePadController* controller) { return controller->axes[R2] > 0 && controller->prevAxes[L2] <= 0; }
inline bool OPgamePadRightTriggerWasReleased(OPgamePadController* controller) { return controller->axes[R2] <= 0 && controller->prevAxes[L2] > 0; }
inline bool OPgamePadRightTriggerIsDown(OPgamePadController* controller) { return controller->axes[R2] > 0; }
inline bool OPgamePadRightTriggerIsUp(OPgamePadController* controller) { return controller->axes[R2] <= 0; }

inline bool OPgamePadIsConnected(OPgamePadController* controller) {
	return controller->connected;
}

inline void OPgamePadSetDeadzone(OPgamePadController* controller, OPfloat deadzone) {
	controller->deadzone = deadzone;
}

inline bool OPgamePadAnyPrevInputIsDown(OPgamePadController* controller) {
	ui32 count = GamePadButton_Max;
	for (; count > 0; count--) {
		if (controller->prevButtons[count]) return true;
	}
	return false;
}

inline bool OPgamePadAnyPrevInputIsUp(OPgamePadController* controller) {
	return !OPgamePadAnyPrevInputIsDown(controller);
}

inline bool OPgamePadAnyInputIsDown(OPgamePadController* controller) {
	ui32 count = GamePadButton_Max;
	for (; count > 0; count--) {
		if (controller->buttons[count]) return true;
	}
	return false;
}

inline bool OPgamePadAnyControllerInputIsDown() {
	ui32 count = 3;
	for (; count > 0; count--) {
		if (OPgamePadAnyInputIsDown(OPgamePad((GamePadIndex)count))) return true;
	}
	return false;
}

inline bool OPgamePadAnyInputIsUp(OPgamePadController* controller) {
	return !OPgamePadAnyInputIsDown(controller);
}

inline bool OPgamePadAnyInputWasPressed(OPgamePadController* controller) {
	return OPgamePadAnyInputIsDown(controller) && OPgamePadAnyPrevInputIsUp(controller);
}
inline bool OPgamePadAnyInputWasReleased(OPgamePadController* controller) {
	return OPgamePadAnyInputIsUp(controller) && OPgamePadAnyPrevInputIsDown(controller);
}

#endif