
#include "./Human/include/Platform/opengles/OPwindowAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPmonitorAPIGLES.h"
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"

ui32 OPmouseMappingGLES[(ui32)OPmouseButton::_MAX];
ui32 OPkeyboardMappingGLES[(ui32)OPkeyboardKey::_MAX];

// TODO: (garrett) CMake should be generating these variables and we'll check for just OPIFEX_OPENGL and use the OPIFEX_OPENGL_MAJOR and OPIFEX_OPENGL_MINOR
#define OPIFEX_OPENGL 1
#define OPIFEX_OPENGL_MAJOR 2
#define OPIFEX_OPENGL_MINOR 0

OPwindow* OPwindowGLESInit(OPwindow* window, OPmonitor* monitor, OPwindowParameters windowParameters) {

	// initialize OpenGL ES and EGL

	OPwindowGLES* windowGL = OPNEW(OPwindowGLES());
	window->internalPtr = windowGL;
	

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
	 const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires.
	* find the best match if possible, otherwise use the very first one
	*/
	eglChooseConfig(display, attribs, NULL,0, &numConfigs);
	EGLConfig* supportedConfigs = OPALLOC(EGLConfig, numConfigs);
	//std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
	//assert(supportedConfigs);
	eglChooseConfig(display, attribs, supportedConfigs, numConfigs, &numConfigs);
	//assert(numConfigs);
	auto i = 0;
	for (; i < numConfigs; i++) {
		auto& cfg = supportedConfigs[i];
		EGLint r, g, b, d;
		if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
			eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
			eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
			eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
			r == 8 && g == 8 && b == 8 && d == 0 ) {

			config = supportedConfigs[i];
			break;
		}
	}
	if (i == numConfigs) {
		config = supportedConfigs[0];
	}
	EGLint contextAttrs[] = {
        EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE
	}; // , EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	surface = eglCreateWindowSurface(display, config, OPAndroidState->window, NULL);
	context = eglCreateContext(display, config, NULL, contextAttrs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		OPlogErr("Unable to eglMakeCurrent");
		return NULL;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	windowGL->display = display;
	windowGL->surface = surface;
	// engine->display = display;
	// engine->context = context;
	// engine->surface = surface;
	// engine->width = w;
	// engine->height = h;
	// engine->state.angle = 0;

	window->WindowWidth = w;
	window->WindowHeight = h;

	//int w2, h2;
	//glfwGetFramebufferSize(windowGL->Handle, &w2, &h2);
	window->WidthScale = w / (f32)window->WindowWidth;
	window->HeightScale = h / (f32)window->WindowHeight;
	window->Width = window->WindowWidth;
	window->Height = window->WindowHeight;
	window->WindowWidth *= window->WidthScale;
	window->WindowHeight *= window->HeightScale;

	// Check openGL on the system
	ui32 opengl_info[4] = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
	for (auto name : opengl_info) {
		auto info = glGetString(name);
		OPlogInfo("OpenGL Info: %s", info);
	}
	// Initialize GL state.
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);


	return NULL;
}

OPwindow* OPwindowGLESCreate(OPmonitor* monitor, OPwindowParameters windowParameters) {
	OPwindow* window = (OPwindow*)OPalloc(sizeof(OPwindow));
	return OPwindowGLESInit(window, monitor, windowParameters);
}

OPwindowParameters OPwindowDefaultParametersGLES() {
	return OPwindowParameters("OPifex Engine", false);
}

void OPwindowGLESBind(OPwindow* window) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwMakeContextCurrent(windowGL->Handle);

	OPrenderSetViewport(0, 0, (OPuint)window->WindowWidth, (OPuint)window->WindowHeight);

	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE = window;
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE == window, "How can this even happen?");
}

void OPwindowSetPositionGLES(OPwindow* window, i32 x, i32 y) {
	OPwindowGLESBind(window);
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwSetWindowPos(windowGL->Handle, x, y);
}

void OPwindowFocusGLES(OPwindow* window) {
	OPwindowGLESBind(window);
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwFocusWindow(windowGL->Handle);
}

OPint OPwindowGLESUpdate(OPwindow* window) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwPollEvents();
	//if (glfwWindowShouldClose(windowGL->Handle)) {
	//	return 1;
	//}
	return 0;
}

OPvec2 OPwindowGLESGetCursorPos(OPwindow* window) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	d64 x, y;
	//glfwGetCursorPos(windowGL->Handle, &x, &y);
	return OPvec2((f32)x, (f32)y);
}

void OPwindowGLESSetCursorPos(OPwindow* window, OPvec2 pos) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwSetCursorPos(windowGL->Handle, pos.x, pos.y);
}

bool OPwindowGLESGetButtonState(OPwindow* window, OPmouseButton mouseButton) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//return glfwGetMouseButton(windowGL->Handle, OPmouseMappingGL[(ui32)mouseButton]) == GLFW_PRESS;
	return false;
}


bool OPwindowGLESGetKeyboardState(OPwindow* window, OPkeyboardKey keyboardkey) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//return glfwGetKey(windowGL->Handle, OPkeyboardMappingGL[(ui32)keyboardkey]) == GLFW_PRESS;
	return false;
}

void OPwindowGLESUnbind(OPwindow* window) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwMakeContextCurrent(NULL);
	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE = NULL;
}

void OPwindowGLESSetDropCallback(OPwindow* window, void(*callback)(OPuint count, const OPchar**)) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	//glfwSetDropCallback(windowGL->Handle, OPwindowGLDrop);
	windowGL->OPWINDOWDROPCALLBACK = callback;
}

void OPwindowGLESDestroy(OPwindow* window) {
	OPwindowGLES* windowGL = (OPwindowGLES*)window->internalPtr;
	OPwindowGLESUnbind(window);
	//glfwDestroyWindow(windowGL->Handle);
	OPfree(windowGL);
	window->internalPtr = NULL;
}

void OPwindowAPIGLESInit(OPwindowAPI* window) {
	window->_Init = OPwindowGLESInit;
	window->_Create = OPwindowGLESCreate;
	window->Bind = OPwindowGLESBind;
	window->DefaultParameters = OPwindowDefaultParametersGLES;
	window->SetPosition = OPwindowSetPositionGLES;
	window->Focus = OPwindowFocusGLES;
	window->Update = OPwindowGLESUpdate;
	window->SetDropCallback = OPwindowGLESSetDropCallback;
	window->GetCursorPos = OPwindowGLESGetCursorPos;
	window->GetButtonState = OPwindowGLESGetButtonState;
	window->GetKeyboardState = OPwindowGLESGetKeyboardState;
	window->Unbind = OPwindowGLESUnbind;
	window->Destroy = OPwindowGLESDestroy;

	// OPmouseMappingGL[(ui32)OPmouseButton::LBUTTON] = GLFW_MOUSE_BUTTON_LEFT;
	// OPmouseMappingGL[(ui32)OPmouseButton::RBUTTON] = GLFW_MOUSE_BUTTON_RIGHT;
	// OPmouseMappingGL[(ui32)OPmouseButton::MBUTTON] = GLFW_MOUSE_BUTTON_MIDDLE;
	// OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_1] = GLFW_MOUSE_BUTTON_1;
	// OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_2] = GLFW_MOUSE_BUTTON_2;
	// OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_3] = GLFW_MOUSE_BUTTON_3;
	// OPmouseMappingGL[(ui32)OPmouseButton::XBUTTON_4] = GLFW_MOUSE_BUTTON_4;

	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::A] = GLFW_KEY_A;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::B] = GLFW_KEY_B;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::C] = GLFW_KEY_C;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::D] = GLFW_KEY_D;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::E] = GLFW_KEY_E;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F] = GLFW_KEY_F;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::G] = GLFW_KEY_G;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::H] = GLFW_KEY_H;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::I] = GLFW_KEY_I;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::J] = GLFW_KEY_J;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::K] = GLFW_KEY_K;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::L] = GLFW_KEY_L;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::M] = GLFW_KEY_M;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N] = GLFW_KEY_N;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::O] = GLFW_KEY_O;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::P] = GLFW_KEY_P;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::Q] = GLFW_KEY_Q;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::R] = GLFW_KEY_R;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::S] = GLFW_KEY_S;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::T] = GLFW_KEY_T;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::U] = GLFW_KEY_U;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::V] = GLFW_KEY_V;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::W] = GLFW_KEY_W;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::X] = GLFW_KEY_X;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::Y] = GLFW_KEY_Y;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::Z] = GLFW_KEY_Z;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N0] = GLFW_KEY_0;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N1] = GLFW_KEY_1;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N2] = GLFW_KEY_2;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N3] = GLFW_KEY_3;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N4] = GLFW_KEY_4;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N5] = GLFW_KEY_5;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N6] = GLFW_KEY_6;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N7] = GLFW_KEY_7;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N8] = GLFW_KEY_8;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::N9] = GLFW_KEY_9;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F1] = GLFW_KEY_F1;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F2] = GLFW_KEY_F2;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F3] = GLFW_KEY_F3;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F4] = GLFW_KEY_F4;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F5] = GLFW_KEY_F5;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F6] = GLFW_KEY_F6;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F7] = GLFW_KEY_F7;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F8] = GLFW_KEY_F8;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F9] = GLFW_KEY_F9;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F10] = GLFW_KEY_F10;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F11] = GLFW_KEY_F11;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F12] = GLFW_KEY_F12;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F13] = GLFW_KEY_F13;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F14] = GLFW_KEY_F14;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F15] = GLFW_KEY_F15;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::F16] = GLFW_KEY_F16;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::BACKSPACE] = GLFW_KEY_BACKSPACE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::TAB] = GLFW_KEY_TAB;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::ENTER] = GLFW_KEY_ENTER;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAUSE] = GLFW_KEY_PAUSE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::CAPSLOCK] = GLFW_KEY_CAPS_LOCK;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::ESCAPE] = GLFW_KEY_ESCAPE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::SPACE] = GLFW_KEY_SPACE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAGEUP] = GLFW_KEY_PAGE_UP;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::PAGEDOWN] = GLFW_KEY_PAGE_DOWN;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::END] = GLFW_KEY_END;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::HOME] = GLFW_KEY_HOME;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::LEFT] = GLFW_KEY_LEFT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::UP] = GLFW_KEY_UP;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::RIGHT] = GLFW_KEY_RIGHT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::DOWN] = GLFW_KEY_DOWN;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::INSERT] = GLFW_KEY_INSERT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::DELETE_] = GLFW_KEY_DELETE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::LWIN] = GLFW_KEY_LEFT_SUPER;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::RWIN] = GLFW_KEY_RIGHT_SUPER;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::MULTIPLY] = GLFW_KEY_KP_MULTIPLY;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::ADD] = GLFW_KEY_KP_ADD;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::SUBTRACT] = GLFW_KEY_KP_SUBTRACT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::DECIMAL] = GLFW_KEY_KP_DECIMAL;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::DIVIDE] = GLFW_KEY_KP_DIVIDE;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMLOCK] = GLFW_KEY_NUM_LOCK;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::SCROLL] = GLFW_KEY_SCROLL_LOCK;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::LSHIFT] = GLFW_KEY_LEFT_SHIFT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::RSHIFT] = GLFW_KEY_RIGHT_SHIFT;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::LCONTROL] = GLFW_KEY_LEFT_CONTROL;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::RCONTROL] = GLFW_KEY_RIGHT_CONTROL;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD0] = GLFW_KEY_KP_0;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD1] = GLFW_KEY_KP_1;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD2] = GLFW_KEY_KP_2;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD3] = GLFW_KEY_KP_3;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD4] = GLFW_KEY_KP_4;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD5] = GLFW_KEY_KP_5;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD6] = GLFW_KEY_KP_6;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD7] = GLFW_KEY_KP_7;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD8] = GLFW_KEY_KP_8;
	// OPkeyboardMappingGL[(ui32)OPkeyboardKey::NUMPAD9] = GLFW_KEY_KP_9;
}
#endif