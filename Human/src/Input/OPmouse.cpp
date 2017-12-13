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

OPmouse OPMOUSE = {
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
void OPmouse::Update() {
    ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");

//#ifndef OPIFEX_DIRECTX_11
//	glfwSetScrollCallback(OPWINDOW_ACTIVE->Window, scrollCB);
//#endif

    OPmemcpy(&prevKeys, &keys, sizeof(bool)* (ui32)OPmouseButton::_MAX);
    prevPositionX = positionX;
    prevPositionY = positionY;
    prevWheel = wheel;

    for(ui32 i = 0; i < (ui32)OPmouseButton::_MAX; i++) {
        keys[i] = OPRENDERER_ACTIVE->Window.GetButtonState(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, (OPmouseButton)i);
    }

    OPvec2 pos = OPRENDERER_ACTIVE->Window.GetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE);
    //OPmouseSetPositionScreenCenter();
    positionX = pos.x;
    positionY = pos.y;
    wheel = updatedWheel;// glfwGetScroll(window);
}

void OPmouse::SetPosition(i32 x, i32 y) {
	OPRENDERER_ACTIVE->Window.SetCursorPos(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, OPvec2((f32)x, (f32)y));
	prevPositionX = x;
	prevPositionY = y;
}

#else
    void OPmouse::Update() {

    }

    void OPmouse::SetPosition(i32 x, i32 y) {

    }
#endif


#include "./Human/include/Rendering/OPwindow.h"
void OPmouse::SetPositionScreenCenter() {
	i32 width = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width;
	i32 height = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height;

	SetPosition((i32)(width / 2), (i32)(height / 2));
}


bool OPmouse::AnyInputIsDown() {
	for (ui32 i = 0; i < (ui32)OPmouseButton::_MAX; i++) {
		if (keys[i]) return true;
	}
	return false;
}
