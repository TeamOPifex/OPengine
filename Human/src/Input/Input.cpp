#include "./Human/include/Input/Input.h"
#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Core/include/Log.h"
#ifndef OPIFEX_ANDROID
	#include <GL/glfw.h>
#endif

KeyboardState Keyboard;

MouseState Mouse = {
	0,
	0,
	0,
	0,
	0,
	0
};

TouchState Touch = {
	false,
	false,
	false,
	0.0,
	0.0,
	0.0,
	0.0
};

#ifndef OPIFEX_ANDROID
void OPkeyboardUpdate() {
	OPmemcpy(&Keyboard.prevKeys, &Keyboard.keys, sizeof(KeyboardState));
	for(ui32 i = 0; i < MaxKeyboardKeys; i++) {
		Keyboard.keys[(KeyboardKey)i] = glfwGetKey((KeyboardKey)i);
	}
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
#else
void OPkeyboardUpdate() {

}

bool OPkeyboardIsDown(KeyboardKey key) {
	return false;
}

bool OPkeyboardIsUp(KeyboardKey key) {
	return true;
}

bool OPkeyboardWasPressed(KeyboardKey key) {
	return false;
}

bool OPkeyboardWasReleased(KeyboardKey key) {
	return false;
}
#endif

#ifndef OPIFEX_ANDROID
void OPmouseUpdate() {
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
	glfwSetMousePos(x, y);
	Mouse.positionX = x;
	Mouse.positionY = y;
}
void OPmouseSetPositionScreenCenter() {
	OPmouseSetPosition(OPrenderWidth / 2, OPrenderHeight / 2);
}


void OPtouchUpdate() {
}

f32 OPtouchPositionX() {
	return Touch.positionX;
}

f32 OPtouchPositionY() {
	return 0;
}

f32 OPtouchPositionMovedX() {
	return 0;
}

f32 OPtouchPositionMovedY() {
	return 0;
}

bool OPtouchIsDown() {
	return false;
}

bool OPtouchIsUp() {
	return true;
}

bool OPtouchWasPressed() {
	return false;
}

bool OPtouchWasReleased() {
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

bool OPmouseIsDown(MouseKey key) {
	return false;
}
bool OPmouseIsUp(MouseKey key) {
	return true;
}
bool OPmouseWasPressed(MouseKey key) {
	return false;
}
bool OPmouseWasReleased(MouseKey key) {
	return false;
}
void OPmouseSetPosition(i32 x, i32 y) {	
}
void OPmouseSetPositionScreenCenter() {

}

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_touch(JNIEnv * env, jobject obj,  jint evt, jfloat x, jfloat y){
	Touch.updated = true;
	if(evt == 1) {
		Touch.tapping = true;
	} else {
		Touch.tapping = false;
	}
	Touch.positionX = x;
	Touch.positionY = y;
	OPLog("Event! %d %f %f : Prev %f %f : Diff: %f %f", evt, Touch.positionX, Touch.positionY, Touch.prevPositionX, Touch.prevPositionY, OPtouchPositionMovedX(), OPtouchPositionMovedY());
}


void OPtouchUpdate() {
	if(Touch.updated) {
		Touch.prevPositionX = Touch.positionX;
		Touch.prevPositionY = Touch.positionY;
		Touch.prevTapping = Touch.tapping;
		Touch.updated = false;
	}
}

f32 OPtouchPositionX() {
	return Touch.positionX;
}

f32 OPtouchPositionY() {
	return Touch.positionY;
}

f32 OPtouchPositionMovedX() {
	return Touch.prevPositionX - Touch.positionX;
}

f32 OPtouchPositionMovedY() {
	return Touch.prevPositionY - Touch.positionY;
}

bool OPtouchIsDown() {
	return Touch.tapping;
}

bool OPtouchIsUp() {
	return !Touch.tapping;
}

bool OPtouchWasPressed() {
	return Touch.tapping && !Touch.prevTapping;
}

bool OPtouchWasReleased() {
	return !Touch.tapping && Touch.prevTapping;
}
#endif