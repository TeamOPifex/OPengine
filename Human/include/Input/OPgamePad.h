#ifndef OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM
#define OPENGINE_HUMAN_INPUT_GAMEPAD_SYSTEM

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec2.h"
#include "Enums/OPgamePadAxes.h"
#include "Enums/OPgamePadButtons.h"
#include "Enums/OPgamePadIndices.h"
#include "./Core/include/Assert.h"

#define CONTROLLERS 4

struct OPgamePad {
	OPgamePad(i8 index) {
		controllerIndex = index;
		connected = false;
		deadzone = 0.1f;
	}

	i8 controllerIndex;
	OPfloat axes[_OPGAMEPADAXIS_MAX];
	OPfloat prevAxes[_OPGAMEPADAXIS_MAX];
	OPfloat deadzone;
	bool connected;
	bool buttons[_OPGAMEPADBUTTON_MAX];
	bool prevButtons[_OPGAMEPADBUTTON_MAX];

	void Update();
	void Reset();

	OPvec2 LeftThumb();
	OPvec2 RightThumb();
	
	OPint IsConnected() {
		return connected;
	}

	void SetDeadzone(OPfloat deadzone) {
		this->deadzone = deadzone;
	}

	// Buttons

	bool IsDown(OPgamePadButton button) {
		return buttons[button];
	}
	bool IsUp(OPgamePadButton button) {
		return !buttons[button];
	}
	bool WasPressed(OPgamePadButton button) {
		return buttons[button] && !prevButtons[button];
	}
	bool WasReleased(OPgamePadButton button) {
		return !buttons[button] && prevButtons[button];
	}

	// Left Thumbstick

	bool LeftThumbIsRight() { 
		return axes[OPGAMEPADAXIS_LS_X] > deadzone; 
	}
	bool LeftThumbNowRight() { 
		return prevAxes[OPGAMEPADAXIS_LS_X] <= deadzone && axes[OPGAMEPADAXIS_LS_X] > deadzone; 
	}
	bool LeftThumbWasRight() { 
		return prevAxes[OPGAMEPADAXIS_LS_X] > deadzone && axes[OPGAMEPADAXIS_LS_X] <= deadzone; 
	}
	bool LeftThumbIsLeft() { 
		return axes[OPGAMEPADAXIS_LS_X] < -deadzone; 
	}
	bool LeftThumbNowLeft() { 
		return prevAxes[OPGAMEPADAXIS_LS_X] >= -deadzone && axes[OPGAMEPADAXIS_LS_X] < -deadzone; 
	}
	bool LeftThumbWasLeft() { 
		return prevAxes[OPGAMEPADAXIS_LS_X] < -deadzone && axes[OPGAMEPADAXIS_LS_X] >= -deadzone; 
	}
	bool LeftThumbIsUp() { 
		return axes[OPGAMEPADAXIS_LS_Y] > deadzone; 
	}
	bool LeftThumbNowUp() { 
		return prevAxes[OPGAMEPADAXIS_LS_Y] <= deadzone && axes[OPGAMEPADAXIS_LS_Y] > deadzone; 
	}
	bool LeftThumbWasUp() { 
		return prevAxes[OPGAMEPADAXIS_LS_Y] > deadzone && axes[OPGAMEPADAXIS_LS_Y] <= deadzone; 
	}
	bool LeftThumbIsDown() { 
		return axes[OPGAMEPADAXIS_LS_Y] < -deadzone; 
	}
	bool LeftThumbNowDown() { 
		return prevAxes[OPGAMEPADAXIS_LS_Y] >= -deadzone && axes[OPGAMEPADAXIS_LS_Y] < -deadzone; 
	}
	bool LeftThumbWasDown() {
		return prevAxes[OPGAMEPADAXIS_LS_Y] < -deadzone && axes[OPGAMEPADAXIS_LS_Y] >= -deadzone; 
	}
	OPfloat LeftThumbX() {
		return axes[OPGAMEPADAXIS_LS_X];
	}
	OPfloat LeftThumbY() {
		return axes[OPGAMEPADAXIS_LS_Y];
	}
	
	// Right Thumbstick

	bool RightThumbIsRight() { 
		return axes[OPGAMEPADAXIS_RS_X] > deadzone; 
	}
	bool RightThumbNowRight() { 
		return prevAxes[OPGAMEPADAXIS_RS_X] <= deadzone && axes[OPGAMEPADAXIS_RS_X] > deadzone; 
	}
	bool RightThumbWasRight() { 
		return prevAxes[OPGAMEPADAXIS_RS_X] > deadzone && axes[OPGAMEPADAXIS_RS_X] <= deadzone; 
	}
	bool RightThumbIsLeft() { 
		return axes[OPGAMEPADAXIS_RS_X] < -deadzone; 
	}
	bool RightThumbNowLeft() { 
		return prevAxes[OPGAMEPADAXIS_RS_X] >= -deadzone && axes[OPGAMEPADAXIS_RS_X] < -deadzone; 
	}
	bool RightThumbWasLeft() { 
		return prevAxes[OPGAMEPADAXIS_RS_X] < -deadzone && axes[OPGAMEPADAXIS_RS_X] >= -deadzone; 
	}
	bool RightThumbIsUp() { 
		return axes[OPGAMEPADAXIS_RS_Y] > deadzone; 
	}
	bool RightThumbNowUp() { 
		return prevAxes[OPGAMEPADAXIS_RS_Y] <= deadzone && axes[OPGAMEPADAXIS_LS_Y] > deadzone; 
	}
	bool RightThumbWasUp() { 
		return prevAxes[OPGAMEPADAXIS_RS_Y] > deadzone && axes[OPGAMEPADAXIS_LS_Y] <= deadzone; 
	}
	bool RightThumbIsDown() { 
		return axes[OPGAMEPADAXIS_RS_Y] < -deadzone; 
	}
	bool RightThumbNowDown() { 
		return prevAxes[OPGAMEPADAXIS_RS_Y] >= -deadzone && axes[OPGAMEPADAXIS_LS_Y] < -deadzone; 
	}
	bool RightThumbWasDown() { 
		return prevAxes[OPGAMEPADAXIS_RS_Y] < -deadzone && axes[OPGAMEPADAXIS_LS_Y] >= -deadzone; 
	}
	OPfloat RightThumbX() {
		return axes[OPGAMEPADAXIS_RS_X];
	}
	OPfloat RightThumbY() {
		return axes[OPGAMEPADAXIS_RS_Y];
	}

	// Left Trigger

	OPfloat LeftTrigger() {
		return axes[OPGAMEPADAXIS_L2];
	}
	bool LeftTriggerWasPressed() { 
		return axes[OPGAMEPADAXIS_L2] > 0 && prevAxes[OPGAMEPADAXIS_L2] <= 0; 
	}
	bool LeftTriggerWasReleased() { 
		return axes[OPGAMEPADAXIS_L2] <= 0 && prevAxes[OPGAMEPADAXIS_L2] > 0; 
	}
	bool LeftTriggerIsDown() { 
		return axes[OPGAMEPADAXIS_L2] > 0; 
	}
	bool LeftTriggerIsUp() { 
		return axes[OPGAMEPADAXIS_L2] <= 0; 
	}

	// Right Trigger

	OPfloat RightTrigger() {
		return axes[OPGAMEPADAXIS_R2];
	}
	bool RightTriggerWasPressed() { 
		return axes[OPGAMEPADAXIS_R2] > 0 && prevAxes[OPGAMEPADAXIS_R2] <= 0;
	}
	bool RightTriggerWasReleased() { 
		return axes[OPGAMEPADAXIS_R2] <= 0 && prevAxes[OPGAMEPADAXIS_L2] > 0;
	}
	bool RightTriggerIsDown() { 
		return axes[OPGAMEPADAXIS_R2] > 0; 
	}
	bool RightTriggerIsUp() { 
		return axes[OPGAMEPADAXIS_R2] <= 0; 
	}

	bool AnyPrevInputIsDown();
	bool AnyPrevInputIsUp() {
		return !AnyPrevInputIsDown();
	}	
	bool AnyInputIsDown();
	bool AnyInputIsUp() {
		return !AnyInputIsDown();
	}
	bool AnyInputWasPressed() {
		return AnyInputIsDown() && AnyPrevInputIsUp();
	}
	bool AnyInputWasReleased() {
		return AnyInputIsUp() && AnyPrevInputIsDown();
	}
};
typedef struct OPgamePad OPgamePad;

struct OPgamePadSystem {
	OPgamePad gamePads[CONTROLLERS];

	void Update();
	void Reset();
	void SetDeadzones(OPfloat deadzone);
	OPgamePad* Get(OPgamePadIndex index);
	OPgamePad* Get(i8 index) {
		ASSERT(index < CONTROLLERS - 1, "Trying to access a controller that's not indexed, see CONTROLLERS global");
		return &gamePads[index];
	}
	
	bool AnyControllerInputIsDown() {
		ui32 count = CONTROLLERS - 1;
		for (; count > 0; count--) {
			if (gamePads[count].AnyInputIsDown()) return true;
		}
		return false;
	}
};
typedef struct OPgamePadSystem OPgamePadSystem;

extern OPgamePadSystem OPGAMEPADSYSTEM;


inline OPgamePad* OPgamePadGet(OPgamePadIndex index) {
	return OPGAMEPADSYSTEM.Get(index);
}
inline OPgamePad* OPgamePadGet(i8 index) {
	return OPGAMEPADSYSTEM.Get(index);
}


#endif
