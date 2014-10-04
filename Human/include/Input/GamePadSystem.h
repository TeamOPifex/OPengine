#ifndef OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM
#define OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM

#include "./Core/include/Types.h"
#include "./Math/include/Vector2.h"
#include "GamePadEnums.h"

#define CONTROLLERS 4

typedef struct {
	OPint		playerIndex;
	OPint           connected;
	OPfloat		deadzone;
	OPint           buttons[GamePadButton_Max];
	OPint           prevButtons[GamePadButton_Max];
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

inline OPint OPgamePadIsDown(OPgamePadController* controller, GamePadButton button){
	return controller->buttons[button];
}

inline OPint OPgamePadIsUp(OPgamePadController* controller, GamePadButton button){
	return !controller->buttons[button];
}

inline OPint OPgamePadWasPressed(OPgamePadController* controller, GamePadButton button){
	return controller->buttons[button] && !controller->prevButtons[button];
}

inline OPint OPgamePadWasReleased(OPgamePadController* controller, GamePadButton button){
	return !controller->buttons[button] && controller->prevButtons[button];
}

inline OPint OPgamePadLeftThumbIsRight(OPgamePadController* controller) { return controller->axes[LS_X] > controller->deadzone; }
inline OPint OPgamePadLeftThumbNowRight(OPgamePadController* controller) { return controller->prevAxes[LS_X] <= controller->deadzone && controller->axes[LS_X] > controller->deadzone; }
inline OPint OPgamePadLeftThumbWasRight(OPgamePadController* controller) { return controller->prevAxes[LS_X] > controller->deadzone && controller->axes[LS_X] <= controller->deadzone; }
inline OPint OPgamePadLeftThumbIsLeft(OPgamePadController* controller) { return controller->axes[LS_X] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbNowLeft(OPgamePadController* controller) { return controller->prevAxes[LS_X] >= -controller->deadzone && controller->axes[LS_X] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbWasLeft(OPgamePadController* controller) { return controller->prevAxes[LS_X] < -controller->deadzone && controller->axes[LS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbX(OPgamePadController* controller) {
	return controller->axes[LS_X];
}

inline OPint OPgamePadLeftThumbIsUp(OPgamePadController* controller) { return controller->axes[LS_Y] > controller->deadzone; }
inline OPint OPgamePadLeftThumbNowUp(OPgamePadController* controller) { return controller->prevAxes[LS_Y] <= controller->deadzone && controller->axes[LS_Y] > controller->deadzone; }
inline OPint OPgamePadLeftThumbWasUp(OPgamePadController* controller) { return controller->prevAxes[LS_Y] > controller->deadzone && controller->axes[LS_Y] <= controller->deadzone; }
inline OPint OPgamePadLeftThumbIsDown(OPgamePadController* controller) { return controller->axes[LS_Y] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbNowDown(OPgamePadController* controller) { return controller->prevAxes[LS_Y] >= -controller->deadzone && controller->axes[LS_Y] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbWasDown(OPgamePadController* controller) { return controller->prevAxes[LS_Y] < -controller->deadzone && controller->axes[LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbY(OPgamePadController* controller) {
	return controller->axes[LS_Y];
}

inline OPint OPgamePadRightThumbIsRight(OPgamePadController* controller) { return controller->axes[RS_X] > controller->deadzone; }
inline OPint OPgamePadRightThumbNowRight(OPgamePadController* controller) { return controller->prevAxes[RS_X] <= controller->deadzone && controller->axes[RS_X] > controller->deadzone; }
inline OPint OPgamePadRightThumbWasRight(OPgamePadController* controller) { return controller->prevAxes[RS_X] > controller->deadzone && controller->axes[RS_X] <= controller->deadzone; }
inline OPint OPgamePadRightThumbIsLeft(OPgamePadController* controller) { return controller->axes[RS_X] < -controller->deadzone; }
inline OPint OPgamePadRightThumbNowLeft(OPgamePadController* controller) { return controller->prevAxes[RS_X] >= -controller->deadzone && controller->axes[RS_X] < -controller->deadzone; }
inline OPint OPgamePadRightThumbWasLeft(OPgamePadController* controller) { return controller->prevAxes[RS_X] < -controller->deadzone && controller->axes[RS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbX(OPgamePadController* controller) {
	return controller->axes[RS_X];
}


inline OPint OPgamePadRightThumbIsUp(OPgamePadController* controller) { return controller->axes[RS_Y] > controller->deadzone; }
inline OPint OPgamePadRightThumbNowUp(OPgamePadController* controller) { return controller->prevAxes[RS_Y] <= controller->deadzone && controller->axes[LS_Y] > controller->deadzone; }
inline OPint OPgamePadRightThumbWasUp(OPgamePadController* controller) { return controller->prevAxes[RS_Y] > controller->deadzone && controller->axes[LS_Y] <= controller->deadzone; }
inline OPint OPgamePadRightThumbIsDown(OPgamePadController* controller) { return controller->axes[RS_Y] < -controller->deadzone; }
inline OPint OPgamePadRightThumbNowDown(OPgamePadController* controller) { return controller->prevAxes[RS_Y] >= -controller->deadzone && controller->axes[LS_Y] < -controller->deadzone; }
inline OPint OPgamePadRightThumbWasDown(OPgamePadController* controller) { return controller->prevAxes[RS_Y] < -controller->deadzone && controller->axes[LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbY(OPgamePadController* controller) {
	return controller->axes[RS_Y];
}

inline OPfloat OPgamePadLeftTrigger(OPgamePadController* controller) {
	return controller->axes[L2];
}
inline OPint OPgamePadLeftTriggerWasPressed(OPgamePadController* controller) { return controller->axes[L2] > 0 && controller->prevAxes[L2] <= 0; }
inline OPint OPgamePadLeftTriggerWasReleased(OPgamePadController* controller) { return controller->axes[L2] <= 0 && controller->prevAxes[L2] > 0; }
inline OPint OPgamePadLeftTriggerIsDown(OPgamePadController* controller) { return controller->axes[L2] > 0; }
inline OPint OPgamePadLeftTriggerIsUp(OPgamePadController* controller) { return controller->axes[L2] <= 0; }

inline OPfloat OPgamePadRightTrigger(OPgamePadController* controller) {
	return controller->axes[R2];
}
inline OPint OPgamePadRightTriggerWasPressed(OPgamePadController* controller) { return controller->axes[R2] > 0 && controller->prevAxes[R2] <= 0; }
inline OPint OPgamePadRightTriggerWasReleased(OPgamePadController* controller) { return controller->axes[R2] <= 0 && controller->prevAxes[L2] > 0; }
inline OPint OPgamePadRightTriggerIsDown(OPgamePadController* controller) { return controller->axes[R2] > 0; }
inline OPint OPgamePadRightTriggerIsUp(OPgamePadController* controller) { return controller->axes[R2] <= 0; }

inline OPint OPgamePadIsConnected(OPgamePadController* controller) {
	return controller->connected;
}

inline void OPgamePadSetDeadzone(OPgamePadController* controller, OPfloat deadzone) {
	controller->deadzone = deadzone;
}

inline OPint OPgamePadAnyPrevInputIsDown(OPgamePadController* controller) {
	ui32 count = GamePadButton_Max;
	for (; count > 0; count--) {
		if (controller->prevButtons[count]) return true;
	}
	return false;
}

inline OPint OPgamePadAnyPrevInputIsUp(OPgamePadController* controller) {
	return !OPgamePadAnyPrevInputIsDown(controller);
}

inline OPint OPgamePadAnyInputIsDown(OPgamePadController* controller) {
	ui32 count = GamePadButton_Max;
	for (; count > 0; count--) {
		if (controller->buttons[count]) return true;
	}
	return false;
}

inline OPint OPgamePadAnyControllerInputIsDown() {
	ui32 count = 3;
	for (; count > 0; count--) {
		if (OPgamePadAnyInputIsDown(OPgamePad((GamePadIndex)count))) return true;
	}
	return false;
}

inline OPint OPgamePadAnyInputIsUp(OPgamePadController* controller) {
	return !OPgamePadAnyInputIsDown(controller);
}

inline OPint OPgamePadAnyInputWasPressed(OPgamePadController* controller) {
	return OPgamePadAnyInputIsDown(controller) && OPgamePadAnyPrevInputIsUp(controller);
}
inline OPint OPgamePadAnyInputWasReleased(OPgamePadController* controller) {
	return OPgamePadAnyInputIsUp(controller) && OPgamePadAnyPrevInputIsDown(controller);
}

#endif
