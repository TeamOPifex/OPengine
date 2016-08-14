#include "./Human/include/Input/OPmouse.h"
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


#include "./Human/include/Rendering/OPrender.h"

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
void OPmouseUpdate() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");

//#ifndef OPIFEX_DIRECTX_11
//	glfwSetScrollCallback(OPWINDOW_ACTIVE->Window, scrollCB);
//#endif

	OPmemcpy(&Mouse.prevKeys, &Mouse.keys, sizeof(OPint)* (ui32)OPmouseButton::_MAX);
	Mouse.prevPositionX = Mouse.positionX;
	Mouse.prevPositionY = Mouse.positionY;
	Mouse.prevWheel = Mouse.wheel;

	for(ui32 i = 0; i < (ui32)OPmouseButton::_MAX; i++) {
		Mouse.keys[i] = OPRENDERER_ACTIVE->Window.GetButtonState(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, (OPmouseButton)i);
	}

	OPvec2 pos = OPRENDERER_ACTIVE->Window.GetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE);
	//OPmouseSetPositionScreenCenter();
	Mouse.positionX = pos.x;
	Mouse.positionY = pos.y;
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

bool OPmouseIsDown(OPmouseButton key) {
	return Mouse.keys[(ui32)key];
}
bool OPmouseIsUp(OPmouseButton key) {
	return !Mouse.keys[(ui32)key];
}
bool OPmouseWasPressed(OPmouseButton key) {
	return Mouse.keys[(ui32)key] && !Mouse.prevKeys[(ui32)key];
}
bool OPmouseWasReleased(OPmouseButton key) {
	return !Mouse.keys[(ui32)key] && Mouse.prevKeys[(ui32)key];
}
void OPmouseSetPosition(i32 x, i32 y) {
	OPRENDERER_ACTIVE->Window.SetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, OPvec2((f32)x, (f32)y));
	Mouse.prevPositionX = x;
	Mouse.prevPositionY = y;
}

#include "./Human/include/Rendering/OPwindow.h"
void OPmouseSetPositionScreenCenter() {
	i32 width = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width;
	i32 height = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height;

	OPmouseSetPosition((i32)(width / 2), (i32)(height / 2));
}

bool OPmouseAnyInputIsDown() {
	for (ui32 i = 0; i < (ui32)OPmouseButton::_MAX; i++) {
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
