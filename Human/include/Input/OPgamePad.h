#ifndef OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM
#define OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec2.h"
#include "Enums/OPgamePadAxes.h"
#include "Enums/OPgamePadButtons.h"
#include "Enums/OPgamePadIndices.h"

#define CONTROLLERS 4

struct OPgamePad {
	OPint		playerIndex;
	OPint           connected;
	OPfloat		deadzone;
	OPint           buttons[_OPGAMEPADBUTTON_MAX];
	OPint           prevButtons[_OPGAMEPADBUTTON_MAX];
	OPfloat		axes[_OPGAMEPADAXIS_MAX];
	OPfloat		prevAxes[_OPGAMEPADAXIS_MAX];
};
typedef struct OPgamePad OPgamePad;

extern OPgamePad GamePads[CONTROLLERS];

OPgamePad* OPgamePadGet(OPgamePadIndex index);
inline OPgamePad* OPgamePadGet(ui32 index) {
	return OPgamePadGet((enum OPgamePadIndex)index);
}

void OPgamePadUpdate(OPgamePad* controller);
void OPgamePadReset(OPgamePad* controller);
void OPgamePadSetDeadzone(OPgamePad* controller, OPfloat deadzone);

void OPgamePadSystemUpdate();
void OPgamePadSystemReset();
void OPgamePadSetDeadZones(OPfloat deadzone);

OPvec2 OPgamePadLeftThumb(OPgamePad* controller);
OPvec2 OPgamePadRightThumb(OPgamePad* controller);

inline OPint OPgamePadIsDown(OPgamePad* controller, OPgamePadButton button){
	return controller->buttons[button];
}

inline OPint OPgamePadIsUp(OPgamePad* controller, OPgamePadButton button){
	return !controller->buttons[button];
}

inline OPint OPgamePadWasPressed(OPgamePad* controller, OPgamePadButton button){
	return controller->buttons[button] && !controller->prevButtons[button];
}

inline OPint OPgamePadWasReleased(OPgamePad* controller, OPgamePadButton button){
	return !controller->buttons[button] && controller->prevButtons[button];
}

inline OPint OPgamePadLeftThumbIsRight(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_LS_X] > controller->deadzone; }
inline OPint OPgamePadLeftThumbNowRight(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_X] <= controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_X] > controller->deadzone; }
inline OPint OPgamePadLeftThumbWasRight(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_X] > controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_X] <= controller->deadzone; }
inline OPint OPgamePadLeftThumbIsLeft(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_LS_X] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbNowLeft(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_X] >= -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_X] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbWasLeft(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_X] < -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbX(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_LS_X];
}

inline OPint OPgamePadLeftThumbIsUp(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_LS_Y] > controller->deadzone; }
inline OPint OPgamePadLeftThumbNowUp(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_Y] <= controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] > controller->deadzone; }
inline OPint OPgamePadLeftThumbWasUp(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_Y] > controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] <= controller->deadzone; }
inline OPint OPgamePadLeftThumbIsDown(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_LS_Y] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbNowDown(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_Y] >= -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] < -controller->deadzone; }
inline OPint OPgamePadLeftThumbWasDown(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_LS_Y] < -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadLeftThumbY(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_LS_Y];
}

inline OPint OPgamePadRightThumbIsRight(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_RS_X] > controller->deadzone; }
inline OPint OPgamePadRightThumbNowRight(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_X] <= controller->deadzone && controller->axes[OPGAMEPADAXIS_RS_X] > controller->deadzone; }
inline OPint OPgamePadRightThumbWasRight(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_X] > controller->deadzone && controller->axes[OPGAMEPADAXIS_RS_X] <= controller->deadzone; }
inline OPint OPgamePadRightThumbIsLeft(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_RS_X] < -controller->deadzone; }
inline OPint OPgamePadRightThumbNowLeft(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_X] >= -controller->deadzone && controller->axes[OPGAMEPADAXIS_RS_X] < -controller->deadzone; }
inline OPint OPgamePadRightThumbWasLeft(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_X] < -controller->deadzone && controller->axes[OPGAMEPADAXIS_RS_X] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbX(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_RS_X];
}


inline OPint OPgamePadRightThumbIsUp(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_RS_Y] > controller->deadzone; }
inline OPint OPgamePadRightThumbNowUp(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_Y] <= controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] > controller->deadzone; }
inline OPint OPgamePadRightThumbWasUp(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_Y] > controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] <= controller->deadzone; }
inline OPint OPgamePadRightThumbIsDown(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_RS_Y] < -controller->deadzone; }
inline OPint OPgamePadRightThumbNowDown(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_Y] >= -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] < -controller->deadzone; }
inline OPint OPgamePadRightThumbWasDown(OPgamePad* controller) { return controller->prevAxes[OPGAMEPADAXIS_RS_Y] < -controller->deadzone && controller->axes[OPGAMEPADAXIS_LS_Y] >= -controller->deadzone; }
inline OPfloat OPgamePadRightThumbY(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_RS_Y];
}

inline OPfloat OPgamePadLeftTrigger(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_L2];
}
inline OPint OPgamePadLeftTriggerWasPressed(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_L2] > 0 && controller->prevAxes[OPGAMEPADAXIS_L2] <= 0; }
inline OPint OPgamePadLeftTriggerWasReleased(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_L2] <= 0 && controller->prevAxes[OPGAMEPADAXIS_L2] > 0; }
inline OPint OPgamePadLeftTriggerIsDown(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_L2] > 0; }
inline OPint OPgamePadLeftTriggerIsUp(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_L2] <= 0; }

inline OPfloat OPgamePadRightTrigger(OPgamePad* controller) {
	return controller->axes[OPGAMEPADAXIS_R2];
}
inline OPint OPgamePadRightTriggerWasPressed(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_R2] > 0 && controller->prevAxes[OPGAMEPADAXIS_R2] <= 0; }
inline OPint OPgamePadRightTriggerWasReleased(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_R2] <= 0 && controller->prevAxes[OPGAMEPADAXIS_L2] > 0; }
inline OPint OPgamePadRightTriggerIsDown(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_R2] > 0; }
inline OPint OPgamePadRightTriggerIsUp(OPgamePad* controller) { return controller->axes[OPGAMEPADAXIS_R2] <= 0; }

inline OPint OPgamePadIsConnected(OPgamePad* controller) {
	return controller->connected;
}

inline void OPgamePadSetDeadzone(OPgamePad* controller, OPfloat deadzone) {
	controller->deadzone = deadzone;
}

inline OPint OPgamePadAnyPrevInputIsDown(OPgamePad* controller) {
	ui32 count = _OPGAMEPADBUTTON_MAX;
	for (; count > 0; count--) {
		if (controller->prevButtons[count]) return true;
	}
	return false;
}

inline OPint OPgamePadAnyPrevInputIsUp(OPgamePad* controller) {
	return !OPgamePadAnyPrevInputIsDown(controller);
}

inline OPint OPgamePadAnyInputIsDown(OPgamePad* controller) {
	ui32 count = _OPGAMEPADBUTTON_MAX;
	for (; count > 0; count--) {
		if (controller->buttons[count]) return true;
	}
	return false;
}

inline OPint OPgamePadAnyControllerInputIsDown() {
	ui32 count = 3;
	for (; count > 0; count--) {
		if (OPgamePadAnyInputIsDown(OPgamePadGet((OPgamePadIndex)count))) return true;
	}
	return false;
}

inline OPint OPgamePadAnyInputIsUp(OPgamePad* controller) {
	return !OPgamePadAnyInputIsDown(controller);
}

inline OPint OPgamePadAnyInputWasPressed(OPgamePad* controller) {
	return OPgamePadAnyInputIsDown(controller) && OPgamePadAnyPrevInputIsUp(controller);
}
inline OPint OPgamePadAnyInputWasReleased(OPgamePad* controller) {
	return OPgamePadAnyInputIsUp(controller) && OPgamePadAnyPrevInputIsDown(controller);
}

#endif
