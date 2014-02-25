#include "./Human/include/Input/Input.h"
#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Core/include/Log.h"
#ifndef OPIFEX_ANDROID
	#include <GLFW/glfw3.h>
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
	OPlog("OPkeyboardUpdate() - entered");
	OPmemcpy(&Keyboard.prevKeys, &Keyboard.keys, sizeof(KeyboardState));
	OPlog("OPkeyboardUpdate() - keys copied");
	for(ui32 i = 0; i < OPKEYBOARD_MAX; i++) {
		Keyboard.keys[i] = glfwGetKey(window, OPkeyboardCodes[i]);
		//if(Keyboard.keys[i] ) {
		//	OPlog("Key %d : %d", i, OPkeyboardCodes[i]);
		//}
	}

	OPlog("OPkeyboardUpdate() - done");
}

OPint OPkeyboardIsDown(OPkeyboardKey key) {
	return Keyboard.keys[key];
}

OPint OPkeyboardIsUp(OPkeyboardKey key) {
	return !Keyboard.keys[key];
}

OPint OPkeyboardWasPressed(OPkeyboardKey key) {
	return Keyboard.keys[key] && !Keyboard.prevKeys[key];
}

OPint OPkeyboardWasReleased(OPkeyboardKey key) {
	return !Keyboard.keys[key] && Keyboard.prevKeys[key];
}

OPint OPkeyboardAnyInputIsDown() {
	for (ui32 i = 0; i < OPKEYBOARD_MAX; i++) {
		if (Keyboard.keys[i]) return true;
	}
	return false;
}
#else
void OPkeyboardUpdate() {

}

OPint OPkeyboardIsDown(OPkeyboardKey key) {
	return false;
}

OPint OPkeyboardIsUp(OPkeyboardKey key) {
	return true;
}

OPint OPkeyboardWasPressed(OPkeyboardKey key) {
	return false;
}

OPint OPkeyboardWasReleased(OPkeyboardKey key) {
	return false;
}
OPint OPkeyboardAnyInputIsDown() {
	return false;
}
#endif

#ifndef OPIFEX_ANDROID
void OPmouseUpdate() {
	OPmemcpy(&Mouse.prevKeys, &Mouse.keys, sizeof(MouseState));
	Mouse.prevPositionX = Mouse.positionX;
	Mouse.prevPositionY = Mouse.positionY;
	Mouse.prevWheel = Mouse.wheel;

	for(ui32 i = 0; i < OPMOUSE_MAX; i++) {
		Mouse.keys[i] = glfwGetMouseButton(window, OPmouseCodes[i]);
		if(Mouse.keys[i]) {
			OPlog("Key %d : %d", i, OPmouseCodes[i]);
		}
	}
	// TODO: Fix this with the callback for GLFW3ß
	//glfwGetCursorPos(window, &Mouse.positionX, &Mouse.positionY);
	//Mouse.wheel = glfwGetMouseWheel(window);
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

OPint OPtouchIsDown() {
	return false;
}

OPint OPtouchIsUp() {
	return true;
}

OPint OPtouchWasPressed() {
	return false;
}

OPint OPtouchWasReleased() {
	return false;
}

OPint OPtouchAnyInputIsDown() {
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

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_touch(JNIEnv * env, jobject obj,  jint evt, jfloat x, jfloat y){
	Touch.updated = true;
	if(evt == 1) {
		Touch.tapping = true;
	} else {
		Touch.tapping = false;
	}
	Touch.positionX = x;
	Touch.positionY = y;
	OPlog("Event! %d %f %f : Prev %f %f : Diff: %f %f", evt, Touch.positionX, Touch.positionY, Touch.prevPositionX, Touch.prevPositionY, OPtouchPositionMovedX(), OPtouchPositionMovedY());
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

OPint OPtouchIsDown() {
	return Touch.tapping;
}

OPint OPtouchIsUp() {
	return !Touch.tapping;
}

OPint OPtouchWasPressed() {
	return Touch.tapping && !Touch.prevTapping;
}

OPint OPtouchWasReleased() {
	return !Touch.tapping && Touch.prevTapping;
}
OPint OPtouchAnyInputIsDown() {
	return Touch.tapping;
}
#endif


ui32 OPmouseCodes[OPMOUSE_MAX] = {
#ifndef OPIFEX_ANDROID
	GLFW_MOUSE_BUTTON_1,	//	LBUTTON
	GLFW_MOUSE_BUTTON_2,	//	RBUTTON
	GLFW_MOUSE_BUTTON_4,	//	CANCEL
	GLFW_MOUSE_BUTTON_3,	//	MBUTTON
	GLFW_MOUSE_BUTTON_5,	//	XBUTTON1
	GLFW_MOUSE_BUTTON_6		//	XBUTTON2
#endif
};

ui32 OPkeyboardCodes[OPKEYBOARD_MAX] = {
#ifndef OPIFEX_ANDROID
   GLFW_KEY_BACKSPACE,	//   Key_BACKSPACE = 0,
   GLFW_KEY_TAB,		//   Key_TAB,
   GLFW_KEY_ENTER,		//   Key_ENTER,
   GLFW_KEY_PAUSE,		//   Key_PAUSE,
   GLFW_KEY_CAPS_LOCK,	//   Key_CAPSLOCK,
   GLFW_KEY_ESCAPE,		//   Key_ESCAPE,
   GLFW_KEY_SPACE,		//   Key_SPACE,
   GLFW_KEY_PAGE_UP,		//   Key_PAGEUP,
   GLFW_KEY_PAGE_DOWN,	//   Key_PAGEDOWN,
   GLFW_KEY_END,		//   Key_END,
   GLFW_KEY_HOME,		//   Key_HOME,
   GLFW_KEY_LEFT,		//   Key_LEFT,
   GLFW_KEY_UP,			//   Key_UP,
   GLFW_KEY_RIGHT,		//   Key_RIGHT,
   GLFW_KEY_DOWN,		//   Key_DOWN,
   GLFW_KEY_INSERT,		//   Key_INSERT,
   GLFW_KEY_DELETE,		//   Key_DELETE,
   GLFW_KEY_KP_0,		//   Key_0 ,
   GLFW_KEY_KP_1,		//   Key_1 ,
   GLFW_KEY_KP_2,		//   Key_2 ,
   GLFW_KEY_KP_3,		//   Key_3 ,
   GLFW_KEY_KP_4,		//   Key_4 ,
   GLFW_KEY_KP_5,		//   Key_5 ,
   GLFW_KEY_KP_6,		//   Key_6 ,
   GLFW_KEY_KP_7,		//   Key_7 ,
   GLFW_KEY_KP_8,		//   Key_8 ,
   GLFW_KEY_KP_9,		//   Key_9 ,
   65,					//   Key_A ,
   66,					//   Key_B,
   67, 					//   Key_C,
   68, 					//   Key_D,
   69, 					//   Key_E,
   70, 					//   Key_F,
   71, 					//   Key_G,
   72, 					//   Key_H,
   73, 					//   Key_I,
   74, 					//   Key_J,
   75, 					//   Key_K,
   76, 					//   Key_L,
   77, 					//   Key_M,
   78, 					//   Key_N,
   79, 					//   Key_O,
   80, 					//   Key_P,
   81, 					//   Key_Q,
   82, 					//   Key_R,
   83, 					//   Key_S,
   84, 					//   Key_T,
   85, 					//   Key_U,
   86, 					//   Key_V,
   87, 					//   Key_W,
   88, 					//   Key_X,
   89, 					//   Key_Y,
   90, 					//   Key_Z,
   GLFW_KEY_LEFT_SUPER,		//   Key_LWIN,
   GLFW_KEY_RIGHT_SUPER,		//   Key_RWIN,
   GLFW_KEY_KP_0,		//   Key_NUMPAD0,
   GLFW_KEY_KP_1,		//   Key_NUMPAD1,
   GLFW_KEY_KP_2,		//   Key_NUMPAD2,
   GLFW_KEY_KP_3,		//   Key_NUMPAD3,
   GLFW_KEY_KP_4,		//   Key_NUMPAD4,
   GLFW_KEY_KP_5,		//   Key_NUMPAD5,
   GLFW_KEY_KP_6,		//   Key_NUMPAD6,
   GLFW_KEY_KP_7,		//   Key_NUMPAD7,
   GLFW_KEY_KP_8,		//   Key_NUMPAD8,
   GLFW_KEY_KP_9,		//   Key_NUMPAD9,
   GLFW_KEY_KP_MULTIPLY,//   Key_MULTIPLY,
   GLFW_KEY_KP_ADD,		//   Key_ADD,
   GLFW_KEY_KP_SUBTRACT,//   Key_SUBTRACT,
   GLFW_KEY_KP_DECIMAL, //   Key_DECIMAL,
   GLFW_KEY_KP_DIVIDE,	//   Key_DIVIDE,
   GLFW_KEY_F1,			//   Key_F1,
   GLFW_KEY_F2, 		//   Key_F2,
   GLFW_KEY_F3, 		//   Key_F3,
   GLFW_KEY_F4, 		//   Key_F4,
   GLFW_KEY_F5, 		//   Key_F5,
   GLFW_KEY_F6, 		//   Key_F6,
   GLFW_KEY_F7, 		//   Key_F7,
   GLFW_KEY_F8, 		//   Key_F8,
   GLFW_KEY_F9, 		//   Key_F9,
   GLFW_KEY_F10,		//   Key_F10,
   GLFW_KEY_F11, 		//   Key_F11,
   GLFW_KEY_F12, 		//   Key_F12,
   GLFW_KEY_F13, 		//   Key_F13,
   GLFW_KEY_F14, 		//   Key_F14,
   GLFW_KEY_F15, 		//   Key_F15,
   GLFW_KEY_F16, 		//   Key_F16,
   GLFW_KEY_NUM_LOCK,//   Key_NUMLOCK,
   GLFW_KEY_SCROLL_LOCK,//   Key_SCROLL,
   GLFW_KEY_LEFT_SHIFT,		//   Key_LSHIFT,
   GLFW_KEY_RIGHT_SHIFT,		//   Key_RSHIFT,
   GLFW_KEY_LEFT_CONTROL,		//   Key_LCONTROL,
   GLFW_KEY_RIGHT_CONTROL		//   Key_RCONTROL
#endif
};
