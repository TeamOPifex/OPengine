#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

//#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
//	//#include <GLFW/glfw3.h>
//void scrollCB(GLFWwindow* window, double x, double y) {
//    //Mouse.prevWheel = Mouse.wheel;
//    Mouse.updatedWheel += (i32)y;
//    OPlog("Scrolled %f, %f", x, y);
//}
//#endif

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



#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
void OPmouseUpdate() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");

//#ifndef OPIFEX_DIRECTX_11
//	glfwSetScrollCallback(OPWINDOW_ACTIVE->Window, scrollCB);
//#endif

	OPmemcpy(&Mouse.prevKeys, &Mouse.keys, sizeof(OPint)* _OPMOUSE_MAX);
	Mouse.prevPositionX = Mouse.positionX;
	Mouse.prevPositionY = Mouse.positionY;
	Mouse.prevWheel = Mouse.wheel;

	for(ui32 i = 0; i < _OPMOUSE_MAX; i++) {
		Mouse.keys[i] = OPRENDERER_ACTIVE->Window.GetButtonState(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, (OPmouseButton)i);
	}

	// TODO: Fix this with the callback for GLFW3ß
	d64 x, y;
#ifndef OPIFEX_DIRECTX_11
	OPvec2 pos = OPRENDERER_ACTIVE->Window.GetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE);
#endif
	//OPmouseSetPositionScreenCenter();
	Mouse.positionX = x;
	Mouse.positionY = y;
	Mouse.wheel = Mouse.updatedWheel;// glfwGetScroll(window);
}

i32 OPmousePositionX() {
	return (i32)Mouse.positionX;
}
i32 OPmousePositionY() {
	return (i32)Mouse.positionY;
}

i32 OPmousePositionMovedX() {
	return (i32)(Mouse.prevPositionX - Mouse.positionX);
}
i32 OPmousePositionMovedY() {
	return (i32)(Mouse.prevPositionY - Mouse.positionY);
}

i32 OPmouseWheel() {
	return Mouse.wheel;
}
i32 OPmouseWheelMoved() {
	return Mouse.prevWheel - Mouse.wheel;
}

OPint OPmouseIsDown(OPmouseKey key) {
	return Mouse.keys[key];
}
OPint OPmouseIsUp(OPmouseKey key) {
	return !Mouse.keys[key];
}
OPint OPmouseWasPressed(OPmouseKey key) {
	return Mouse.keys[key] && !Mouse.prevKeys[key];
}
OPint OPmouseWasReleased(OPmouseKey key) {
	return !Mouse.keys[key] && Mouse.prevKeys[key];
}
void OPmouseSetPosition(i32 x, i32 y) {	
	OPRENDERER_ACTIVE->Window.SetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, OPvec2(x, y));
	Mouse.prevPositionX = x;
	Mouse.prevPositionY = y;
}
void OPmouseSetPositionScreenCenter() {
	OPmouseSetPosition(
		(i32)((OPRENDER_SCREEN_WIDTH * OPRENDER_SCREEN_WIDTH_SCALE) / 2), 
			(i32)((OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE) / 2));
}
OPint OPmouseAnyInputIsDown() {
	for (ui32 i = 0; i < _OPMOUSE_MAX; i++) {
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

OPint OPmouseIsDown(OPmouseKey key) {
	return false;
}
OPint OPmouseIsUp(OPmouseKey key) {
	return true;
}
OPint OPmouseWasPressed(OPmouseKey key) {
	return false;
}
OPint OPmouseWasReleased(OPmouseKey key) {
	return false;
}
void OPmouseSetPosition(i32 x, i32 y) {	
}
void OPmouseSetPositionScreenCenter() {

}

#endif

//ui32 OPmouseMapping[_OPMOUSE_MAX] = {
//#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
//	GLFW_MOUSE_BUTTON_1,	//	LBUTTON
//	GLFW_MOUSE_BUTTON_2,	//	RBUTTON
//	GLFW_MOUSE_BUTTON_4,	//	CANCEL
//	GLFW_MOUSE_BUTTON_3,	//	MBUTTON
//	GLFW_MOUSE_BUTTON_5,	//	XBUTTON1
//	GLFW_MOUSE_BUTTON_6		//	XBUTTON2
//#endif
//};
