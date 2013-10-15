#include "./Human/include/Input/Input.h"
#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Core/include/Log.h"
#ifndef OPIFEX_ANDROID
	#include <GL/glfw.h>
#endif

KeyboardState Keyboard;

MouseState Mouse;


void OPkeyboardUpdate() {
#ifndef OPIFEX_ANDROID
	OPmemcpy(&Keyboard.prevKeys, &Keyboard.keys, sizeof(KeyboardState));
	for(ui32 i = 0; i < MaxKeyboardKeys; i++) {
		Keyboard.keys[(KeyboardKey)i] = glfwGetKey((KeyboardKey)i);
	}
#endif
}

bool OPkeyboardIsDown(KeyboardKey key) {
	return Keyboard.keys[key];
}

bool OPkeyboardIsUp(KeyboardKey key) {
	return !Keyboard.keys[key];
}

bool OPkeyboardWasPressed(KeyboardKey key) {
	return Keyboard.keys[key] && !Keyboard.prevKeys[key];
}

bool OPkeyboardWasReleased(KeyboardKey key) {
	return !Keyboard.keys[key] && Keyboard.prevKeys[key];
}


void OPmouseUpdate() {
#ifndef OPIFEX_ANDROID
	OPmemcpy(&Mouse.prevKeys, &Mouse.keys, sizeof(MouseState));
	Mouse.prevPositionX = Mouse.positionX;
	Mouse.prevPositionY = Mouse.positionY;
	Mouse.prevWheel = Mouse.wheel;

	for(ui32 i = 0; i < MaxMouseKeys; i++) {
		MouseKey key = static_cast<MouseKey>(i);
		Mouse.keys[key] = glfwGetMouseButton(key);
		if(Mouse.keys[i]) {
			OPLog("Key %d : %d", i, key);
		}
	}
	glfwGetMousePos(&Mouse.positionX, &Mouse.positionY);
	Mouse.wheel = glfwGetMouseWheel();
#endif
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

bool OPmouseIsDown(MouseKey key) {
	return Mouse.keys[key];
}
bool OPmouseIsUp(MouseKey key) {
	return !Mouse.keys[key];
}
bool OPmouseWasPressed(MouseKey key) {
	return Mouse.keys[key] && !Mouse.prevKeys[key];
}
bool OPmouseWasReleased(MouseKey key) {
	return !Mouse.keys[key] && Mouse.prevKeys[key];
}
void OPmouseSetPosition(i32 x, i32 y) {
	
#ifndef OPIFEX_ANDROID
	glfwSetMousePos(x, y);
#endif
	Mouse.positionX = x;
	Mouse.positionY = y;
}
void OPmouseSetPositionScreenCenter() {
	OPmouseSetPosition(OPrenderWidth / 2, OPrenderHeight / 2);
}