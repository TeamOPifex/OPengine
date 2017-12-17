
#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPmonitorAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"

ui32 OPmouseMappingGL[(ui32)OPmouseButton::_MAX];
ui32 OPkeyboardMappingGL[(ui32)OPkeyboardKey::_MAX];

// TODO: (garrett) CMake should be generating these variables and we'll check for just OPIFEX_OPENGL and use the OPIFEX_OPENGL_MAJOR and OPIFEX_OPENGL_MINOR
#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif

void glfwScrollCallback(GLFWwindow* window, double x, double y) {
	OPMOUSE.updatedWheel = y;
}

void glfwWindowfocusCallback(GLFWwindow* glfwWindow, int focused) {
	OPwindow* window = (OPwindow*)glfwGetWindowUserPointer(glfwWindow);
	window->focused = focused == GLFW_TRUE;
}

OPwindow* OPwindowGLInit(OPwindow* window, OPmonitor* monitor, OPwindowParameters windowParameters) {
	//ASSERT(windowParameters.fullscreen == false || (windowParameters.fullscreen && monitor != NULL), "To create a fullscreen window, a monitor must be declared");

	OPwindowGL* windowGL = OPNEW(OPwindowGL());
	window->internalPtr = windowGL;

	GLFWmonitor* display = NULL;
	if (monitor != NULL) {
		OPmonitorGL* monitorGL = (OPmonitorGL*)monitor->internalPtr;
		display = monitorGL->Handle;
	}

	OPint result = glfwInit();
	if (!result) {
		OPlogErr("Failed to initialize GLFW");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPIFEX_OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPIFEX_OPENGL_MINOR);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef OPIFEX_OSX
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_DECORATED, !windowParameters.borderless);
	windowGL->Handle = glfwCreateWindow(windowParameters.width, windowParameters.height, windowParameters.title, display, NULL);
	glfwSetWindowUserPointer(windowGL->Handle, window);

	//glfwSetCharCallback(window, glfwCharacterCallback);
	//glfwSetDropCallback(window, glfwWindowDropCallback);
	glfwSetScrollCallback(windowGL->Handle, glfwScrollCallback);
	glfwSetWindowFocusCallback(windowGL->Handle, glfwWindowfocusCallback);

	ASSERT(windowGL->Handle != NULL, "Unable to create the window.");

	glfwSetInputMode(windowGL->Handle, GLFW_STICKY_KEYS, true);

	window->WindowWidth = windowParameters.width;
	window->WindowHeight = windowParameters.height;

	int w, h;
	glfwGetFramebufferSize(windowGL->Handle, &w, &h);
	window->WidthScale = w / (f32)window->WindowWidth;
	window->HeightScale = h / (f32)window->WindowHeight;
	window->Width = window->WindowWidth;
	window->Height = window->WindowHeight;
	window->WindowWidth *= window->WidthScale;
	window->WindowHeight *= window->HeightScale;

    OPlogInfo("Frame Size: %d, %d", window->Width, window->Height);
    OPlogInfo("Window Size: %d, %d", window->WindowWidth, window->WindowHeight);
    OPlogInfo("Window Scale: %f, %f", window->WidthScale, window->HeightScale);

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(0);

	return window;
}

OPwindow* OPwindowGLCreate(OPmonitor* monitor, OPwindowParameters windowParameters) {
	OPwindow* window = (OPwindow*)OPalloc(sizeof(OPwindow));
	return OPwindowGLInit(window, monitor, windowParameters);
}

OPwindowParameters OPwindowDefaultParametersGL() {
	return OPwindowParameters("OPifex Engine", false);
}

void OPwindowGLBind(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwMakeContextCurrent(windowGL->Handle);

	OPrenderSetViewport(0, 0, (OPuint)window->WindowWidth, (OPuint)window->WindowHeight);

	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE = window;
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE == window, "How can this even happen?");
}

void OPwindowSetPositionGL(OPwindow* window, i32 x, i32 y) {
	OPwindowGLBind(window);
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwSetWindowPos(windowGL->Handle, x, y);
}

void OPwindowFocusGL(OPwindow* window) {
	OPwindowGLBind(window);
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwFocusWindow(windowGL->Handle);
}

OPint OPwindowGLUpdate(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwPollEvents();
	if (glfwWindowShouldClose(windowGL->Handle)) {
		return 1;
	}
	return 0;
}

OPvec2 OPwindowGLGetCursorPos(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	d64 x, y;
	glfwGetCursorPos(windowGL->Handle, &x, &y);
	return OPvec2((f32)x, (f32)y);
}

void OPwindowGLSetCursorPos(OPwindow* window, OPvec2 pos) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwSetCursorPos(windowGL->Handle, pos.x, pos.y);
}

bool OPwindowGLGetButtonState(OPwindow* window, OPmouseButton mouseButton) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	return glfwGetMouseButton(windowGL->Handle, OPmouseMappingGL[(ui32)mouseButton]) == GLFW_PRESS;
}


bool OPwindowGLGetKeyboardState(OPwindow* window, OPkeyboardKey keyboardkey) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	return glfwGetKey(windowGL->Handle, OPkeyboardMappingGL[(ui32)keyboardkey]) == GLFW_PRESS;
}

void OPwindowGLUnbind(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwMakeContextCurrent(NULL);
	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE = NULL;
}

void OPwindowGLDrop(GLFWwindow* windowGLFW, int count, const char** names) {
	void* windowPtr = glfwGetWindowUserPointer(windowGLFW);
	if (windowPtr == NULL) return;
	OPwindow* window = (OPwindow*)windowPtr;
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	if (windowGL->OPWINDOWDROPCALLBACK == NULL) return;
	windowGL->OPWINDOWDROPCALLBACK(count, names);
}

void OPwindowGLSetDropCallback(OPwindow* window, void(*callback)(OPuint count, const OPchar**)) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	glfwSetDropCallback(windowGL->Handle, OPwindowGLDrop);
	windowGL->OPWINDOWDROPCALLBACK = callback;
}

void OPwindowGLDestroy(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	OPwindowGLUnbind(window);
	glfwDestroyWindow(windowGL->Handle);
	OPfree(windowGL);
	window->internalPtr = NULL;
}

void OPwindowAPIGLInit(OPwindowAPI* window) {
	window->_Init = OPwindowGLInit;
	window->_Create = OPwindowGLCreate;
	window->Bind = OPwindowGLBind;
	window->DefaultParameters = OPwindowDefaultParametersGL;
	window->SetPosition = OPwindowSetPositionGL;
	window->Focus = OPwindowFocusGL;
	window->Update = OPwindowGLUpdate;
	window->SetDropCallback = OPwindowGLSetDropCallback;
	window->GetCursorPos = OPwindowGLGetCursorPos;
	window->GetButtonState = OPwindowGLGetButtonState;
	window->GetKeyboardState = OPwindowGLGetKeyboardState;
	window->Unbind = OPwindowGLUnbind;
	window->Destroy = OPwindowGLDestroy;

	OPmouseMappingGL[(ui32)OPmouseButton::LBUTTON] = GLFW_MOUSE_BUTTON_LEFT;
	OPmouseMappingGL[(ui32)OPmouseButton::RBUTTON] = GLFW_MOUSE_BUTTON_RIGHT;
	OPmouseMappingGL[(ui32)OPmouseButton::MBUTTON] = GLFW_MOUSE_BUTTON_MIDDLE;
	OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_1] = GLFW_MOUSE_BUTTON_1;
	OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_2] = GLFW_MOUSE_BUTTON_2;
	OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_3] = GLFW_MOUSE_BUTTON_3;
	OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_4] = GLFW_MOUSE_BUTTON_4;

	OPkeyboardMappingGL[(ui32)OPkeyboardKey::A] = GLFW_KEY_A;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::B] = GLFW_KEY_B;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::C] = GLFW_KEY_C;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::D] = GLFW_KEY_D;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::E] = GLFW_KEY_E;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F] = GLFW_KEY_F;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::G] = GLFW_KEY_G;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::H] = GLFW_KEY_H;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::I] = GLFW_KEY_I;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::J] = GLFW_KEY_J;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::K] = GLFW_KEY_K;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::L] = GLFW_KEY_L;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::M] = GLFW_KEY_M;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N] = GLFW_KEY_N;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::O] = GLFW_KEY_O;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::P] = GLFW_KEY_P;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::Q] = GLFW_KEY_Q;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::R] = GLFW_KEY_R;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::S] = GLFW_KEY_S;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::T] = GLFW_KEY_T;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::U] = GLFW_KEY_U;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::V] = GLFW_KEY_V;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::W] = GLFW_KEY_W;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::X] = GLFW_KEY_X;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::Y] = GLFW_KEY_Y;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::Z] = GLFW_KEY_Z;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N0] = GLFW_KEY_0;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N1] = GLFW_KEY_1;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N2] = GLFW_KEY_2;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N3] = GLFW_KEY_3;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N4] = GLFW_KEY_4;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N5] = GLFW_KEY_5;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N6] = GLFW_KEY_6;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N7] = GLFW_KEY_7;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N8] = GLFW_KEY_8;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::N9] = GLFW_KEY_9;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F1] = GLFW_KEY_F1;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F2] = GLFW_KEY_F2;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F3] = GLFW_KEY_F3;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F4] = GLFW_KEY_F4;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F5] = GLFW_KEY_F5;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F6] = GLFW_KEY_F6;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F7] = GLFW_KEY_F7;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F8] = GLFW_KEY_F8;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F9] = GLFW_KEY_F9;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F10] = GLFW_KEY_F10;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F11] = GLFW_KEY_F11;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F12] = GLFW_KEY_F12;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F13] = GLFW_KEY_F13;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F14] = GLFW_KEY_F14;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F15] = GLFW_KEY_F15;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::F16] = GLFW_KEY_F16;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::BACKSPACE] = GLFW_KEY_BACKSPACE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::TAB] = GLFW_KEY_TAB;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::ENTER] = GLFW_KEY_ENTER;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAUSE] = GLFW_KEY_PAUSE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::CAPSLOCK] = GLFW_KEY_CAPS_LOCK;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::ESCAPE] = GLFW_KEY_ESCAPE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::SPACE] = GLFW_KEY_SPACE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAGEUP] = GLFW_KEY_PAGE_UP;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAGEDOWN] = GLFW_KEY_PAGE_DOWN;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::END] = GLFW_KEY_END;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::HOME] = GLFW_KEY_HOME;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::LEFT] = GLFW_KEY_LEFT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::UP] = GLFW_KEY_UP;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::RIGHT] = GLFW_KEY_RIGHT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::DOWN] = GLFW_KEY_DOWN;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::INSERT] = GLFW_KEY_INSERT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::DELETE_] = GLFW_KEY_DELETE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::LWIN] = GLFW_KEY_LEFT_SUPER;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::RWIN] = GLFW_KEY_RIGHT_SUPER;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::MULTIPLY] = GLFW_KEY_KP_MULTIPLY;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::ADD] = GLFW_KEY_KP_ADD;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::SUBTRACT] = GLFW_KEY_KP_SUBTRACT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::DECIMAL] = GLFW_KEY_KP_DECIMAL;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::DIVIDE] = GLFW_KEY_KP_DIVIDE;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMLOCK] = GLFW_KEY_NUM_LOCK;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::SCROLL] = GLFW_KEY_SCROLL_LOCK;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::LSHIFT] = GLFW_KEY_LEFT_SHIFT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::RSHIFT] = GLFW_KEY_RIGHT_SHIFT;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::LCONTROL] = GLFW_KEY_LEFT_CONTROL;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::RCONTROL] = GLFW_KEY_RIGHT_CONTROL;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD0] = GLFW_KEY_KP_0;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD1] = GLFW_KEY_KP_1;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD2] = GLFW_KEY_KP_2;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD3] = GLFW_KEY_KP_3;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD4] = GLFW_KEY_KP_4;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD5] = GLFW_KEY_KP_5;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD6] = GLFW_KEY_KP_6;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD7] = GLFW_KEY_KP_7;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD8] = GLFW_KEY_KP_8;
	OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD9] = GLFW_KEY_KP_9;
}

#endif