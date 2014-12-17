#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

#ifndef OPIFEX_ANDROID
	#include <GLFW/glfw3.h>
#endif

OPmouseState Mouse = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

#ifndef OPIFEX_ANDROID
void OPmouseUpdate() {
	OPmemcpy(&Mouse.prevKeys, &Mouse.keys, sizeof(OPint)* OPMOUSE_MAX);
	Mouse.prevPositionX = Mouse.positionX;
	Mouse.prevPositionY = Mouse.positionY;
	Mouse.prevWheel = Mouse.wheel;

	for(ui32 i = 0; i < OPMOUSE_MAX; i++) {
		Mouse.keys[i] = glfwGetMouseButton(window, OPmouseMapping[i]);
		//if(Mouse.keys[i]) {
		//	OPlog("Key %d : %d", i, OPmouseCodes[i]);
		//}
	}
	// TODO: Fix this with the callback for GLFW3ß
	glfwGetCursorPos(window, &Mouse.positionX, &Mouse.positionY);
	//Mouse.wheel = glfwGetScroll(window);
}

i32 OPmousePositionX() {
	return Mouse.positionX;
}
i32 OPmousePositionY() {
	return Mouse.positionY;
}

i32 OPmousePositionMovedX() {
	return Mouse.prevPositionX - Mouse.positionX;
}
i32 OPmousePositionMovedY() {
	return Mouse.prevPositionY - Mouse.positionY;
}

i32 OPmouseWheel() {
	return Mouse.wheel;
}
i32 OPmouseWheelMoved() {
	return Mouse.prevWheel - Mouse.wheel;
}

OPint OPmouseIsDown(OPmouseKeys key) {
	return Mouse.keys[key];
}
OPint OPmouseIsUp(OPmouseKeys key) {
	return !Mouse.keys[key];
}
OPint OPmouseWasPressed(OPmouseKeys key) {
	return Mouse.keys[key] && !Mouse.prevKeys[key];
}
OPint OPmouseWasReleased(OPmouseKeys key) {
	return !Mouse.keys[key] && Mouse.prevKeys[key];
}
void OPmouseSetPosition(i32 x, i32 y) {	
	glfwSetCursorPos(window, x, y);
	Mouse.positionX = x;
	Mouse.positionY = y;
}
void OPmouseSetPositionScreenCenter() {
	OPmouseSetPosition(OPrenderWidth / 2, OPrenderHeight / 2);
}
OPint OPmouseAnyInputIsDown() {
	for (ui32 i = 0; i < OPMOUSE_MAX; i++) {
		if (Mouse.keys[i]) return true;
	}
	return false;
}

#else

void OPmouseUpdate() {
}

i32 OPmousePositionX() {
	return 0;
}
i32 OPmousePositionY() {
	return 0;
}

i32 OPmousePositionMovedX() {
	return 0;
}
i32 OPmousePositionMovedY() {
	return 0;
}

i32 OPmouseWheel() {
	return 0;
}
i32 OPmouseWheelMoved() {
	return 0;
}

OPint OPmouseIsDown(OPmouseKeys key) {
	return false;
}
OPint OPmouseIsUp(OPmouseKeys key) {
	return true;
}
OPint OPmouseWasPressed(OPmouseKeys key) {
	return false;
}
OPint OPmouseWasReleased(OPmouseKeys key) {
	return false;
}
void OPmouseSetPosition(i32 x, i32 y) {	
}
void OPmouseSetPositionScreenCenter() {

}

#endif

ui32 OPmouseMapping[OPMOUSE_MAX] = {
#ifndef OPIFEX_ANDROID
	GLFW_MOUSE_BUTTON_1,	//	LBUTTON
	GLFW_MOUSE_BUTTON_2,	//	RBUTTON
	GLFW_MOUSE_BUTTON_4,	//	CANCEL
	GLFW_MOUSE_BUTTON_3,	//	MBUTTON
	GLFW_MOUSE_BUTTON_5,	//	XBUTTON1
	GLFW_MOUSE_BUTTON_6		//	XBUTTON2
#endif
};
