#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Human/include/Input/OPinputRecord.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	//#include <GLFW/glfw3.h>
#endif

OPkeyboardState Keyboard;

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)

void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboardUpdate(OPtimer* timer) {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");

	OPmemcpy(&Keyboard.prevKeys, &Keyboard.keys, (ui32)OPkeyboardKey::_MAX * sizeof(OPint));
	for(ui32 i = 0; i < (ui32)OPkeyboardKey::_MAX; i++) {
		Keyboard.keys[i] = OPRENDERER_ACTIVE->Window.GetKeyboardState(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, (OPkeyboardKey)i);
	}
}

void OPkeyboardUpdatePost(OPtimer* timer) {
   OPinputRecordUpdate(timer);
}

bool OPkeyboardIsDown(OPkeyboardKey key) {
	return Keyboard.keys[(ui32)key];
}

bool OPkeyboardIsUp(OPkeyboardKey key) {
	return !Keyboard.keys[(ui32)key];
}

bool OPkeyboardWasPressed(OPkeyboardKey key) {
	return Keyboard.keys[(ui32)key] && !Keyboard.prevKeys[(ui32)key];
}

bool OPkeyboardWasReleased(OPkeyboardKey key) {
	return !Keyboard.keys[(ui32)key] && Keyboard.prevKeys[(ui32)key];
}

bool OPkeyboardAnyInputIsDown() {
	for (ui32 i = 0; i < (ui32)OPkeyboardKey::_MAX; i++) {
		if (Keyboard.keys[i]) return true;
	}
	return false;
}
#else

void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboardUpdate(OPtimer* timer) {

}

void OPkeyboardUpdatePost(OPtimer* timer) {

}

OPint OPkeyboardIsDown(enum OPkeyboardKey key) {
	return false;
}

OPint OPkeyboardIsUp(enum OPkeyboardKey key) {
	return true;
}

OPint OPkeyboardWasPressed(enum OPkeyboardKey key) {
	return false;
}

OPint OPkeyboardWasReleased(enum OPkeyboardKey key) {
	return false;
}
OPint OPkeyboardAnyInputIsDown() {
	return false;
}
#endif

//void OPkeyboardKey(OPuint codepoint) {
//   if(OPKEYBOARD_STREAM) {
//      char c = (char)codepoint;
//      OPlog("Totes: %c", c);
//      OPKEYBOARD_STREAM(c);
//   }
//}
//
//ui32 OPkeyboardMapping[_OPKEYBOARD_MAX] = {
//#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
//   GLFW_KEY_BACKSPACE,	//   Key_BACKSPACE = 0,
//   GLFW_KEY_TAB,		//   Key_TAB,
//   GLFW_KEY_ENTER,		//   Key_ENTER,
//   GLFW_KEY_PAUSE,		//   Key_PAUSE,
//   GLFW_KEY_CAPS_LOCK,	//   Key_CAPSLOCK,
//   GLFW_KEY_ESCAPE,		//   Key_ESCAPE,
//   GLFW_KEY_SPACE,		//   Key_SPACE,
//   GLFW_KEY_PAGE_UP,		//   Key_PAGEUP,
//   GLFW_KEY_PAGE_DOWN,	//   Key_PAGEDOWN,
//   GLFW_KEY_END,		//   Key_END,
//   GLFW_KEY_HOME,		//   Key_HOME,
//   GLFW_KEY_LEFT,		//   Key_LEFT,
//   GLFW_KEY_UP,			//   Key_UP,
//   GLFW_KEY_RIGHT,		//   Key_RIGHT,
//   GLFW_KEY_DOWN,		//   Key_DOWN,
//   GLFW_KEY_INSERT,		//   Key_INSERT,
//   GLFW_KEY_DELETE,		//   Key_DELETE,
//   GLFW_KEY_0,		//   Key_0 ,
//   GLFW_KEY_1,		//   Key_1 ,
//   GLFW_KEY_2,		//   Key_2 ,
//   GLFW_KEY_3,		//   Key_3 ,
//   GLFW_KEY_4,		//   Key_4 ,
//   GLFW_KEY_5,		//   Key_5 ,
//   GLFW_KEY_6,		//   Key_6 ,
//   GLFW_KEY_7,		//   Key_7 ,
//   GLFW_KEY_8,		//   Key_8 ,
//   GLFW_KEY_9,		//   Key_9 ,
//   65,					//   Key_A ,
//   66,					//   Key_B,
//   67, 					//   Key_C,
//   68, 					//   Key_D,
//   69, 					//   Key_E,
//   70, 					//   Key_F,
//   71, 					//   Key_G,
//   72, 					//   Key_H,
//   73, 					//   Key_I,
//   74, 					//   Key_J,
//   75, 					//   Key_K,
//   76, 					//   Key_L,
//   77, 					//   Key_M,
//   78, 					//   Key_N,
//   79, 					//   Key_O,
//   80, 					//   Key_P,
//   81, 					//   Key_Q,
//   82, 					//   Key_R,
//   83, 					//   Key_S,
//   84, 					//   Key_T,
//   85, 					//   Key_U,
//   86, 					//   Key_V,
//   87, 					//   Key_W,
//   88, 					//   Key_X,
//   89, 					//   Key_Y,
//   90, 					//   Key_Z,
//   GLFW_KEY_LEFT_SUPER,		//   Key_LWIN,
//   GLFW_KEY_RIGHT_SUPER,		//   Key_RWIN,
//   GLFW_KEY_KP_0,		//   Key_NUMPAD0,
//   GLFW_KEY_KP_1,		//   Key_NUMPAD1,
//   GLFW_KEY_KP_2,		//   Key_NUMPAD2,
//   GLFW_KEY_KP_3,		//   Key_NUMPAD3,
//   GLFW_KEY_KP_4,		//   Key_NUMPAD4,
//   GLFW_KEY_KP_5,		//   Key_NUMPAD5,
//   GLFW_KEY_KP_6,		//   Key_NUMPAD6,
//   GLFW_KEY_KP_7,		//   Key_NUMPAD7,
//   GLFW_KEY_KP_8,		//   Key_NUMPAD8,
//   GLFW_KEY_KP_9,		//   Key_NUMPAD9,
//   GLFW_KEY_KP_MULTIPLY,//   Key_MULTIPLY,
//   GLFW_KEY_KP_ADD,		//   Key_ADD,
//   GLFW_KEY_KP_SUBTRACT,//   Key_SUBTRACT,
//   GLFW_KEY_KP_DECIMAL, //   Key_DECIMAL,
//   GLFW_KEY_KP_DIVIDE,	//   Key_DIVIDE,
//   GLFW_KEY_F1,			//   Key_F1,
//   GLFW_KEY_F2, 		//   Key_F2,
//   GLFW_KEY_F3, 		//   Key_F3,
//   GLFW_KEY_F4, 		//   Key_F4,
//   GLFW_KEY_F5, 		//   Key_F5,
//   GLFW_KEY_F6, 		//   Key_F6,
//   GLFW_KEY_F7, 		//   Key_F7,
//   GLFW_KEY_F8, 		//   Key_F8,
//   GLFW_KEY_F9, 		//   Key_F9,
//   GLFW_KEY_F10,		//   Key_F10,
//   GLFW_KEY_F11, 		//   Key_F11,
//   GLFW_KEY_F12, 		//   Key_F12,
//   GLFW_KEY_F13, 		//   Key_F13,
//   GLFW_KEY_F14, 		//   Key_F14,
//   GLFW_KEY_F15, 		//   Key_F15,
//   GLFW_KEY_F16, 		//   Key_F16,
//   GLFW_KEY_NUM_LOCK,//   Key_NUMLOCK,
//   GLFW_KEY_SCROLL_LOCK,//   Key_SCROLL,
//   GLFW_KEY_LEFT_SHIFT,		//   Key_LSHIFT,
//   GLFW_KEY_RIGHT_SHIFT,		//   Key_RSHIFT,
//   GLFW_KEY_LEFT_CONTROL,		//   Key_LCONTROL,
//   GLFW_KEY_RIGHT_CONTROL		//   Key_RCONTROL
//#endif
//};