#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"
#include "./Human/include/Platform/opengl/OPmonitorAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"

ui32 OPmouseMappingGL[(ui32)OPmouseButton::MAX];
ui32 OPkeyboardMappingGL[(ui32)OPkeyboardKey::MAX];

OPwindow* OPwindowGLInit(OPwindow* window, OPmonitor* monitor, OPwindowParameters windowParameters) {
	//ASSERT(windowParameters.fullscreen == false || (windowParameters.fullscreen && monitor != NULL), "To create a fullscreen window, a monitor must be declared");

	OPwindowGL* windowGL = (OPwindowGL*)OPalloc(sizeof(OPwindowGL));;
	window->internalPtr = windowGL;

	GLFWmonitor* display = NULL;
	if (monitor != NULL) {
		OPmonitorGL* monitorGL = (OPmonitorGL*)monitor->internalPtr;
		display = monitorGL->Handle;
	}

	glfwWindowHint(GLFW_DECORATED, !windowParameters.borderless);
	windowGL->Handle = glfwCreateWindow(windowParameters.width, windowParameters.height, windowParameters.title, display, NULL);

	//glfwSetCharCallback(window, glfwCharacterCallback);
	//glfwSetDropCallback(window, glfwWindowDropCallback);

	ASSERT(windowGL->Handle != NULL, "Unable to create the window.");

	glfwSetInputMode(windowGL->Handle, GLFW_STICKY_KEYS, true);

	window->Width = windowParameters.width;
	window->Height = windowParameters.height;

	int w, h;
	glfwGetFramebufferSize(windowGL->Handle, &w, &h);
	window->WidthScaled = w / (f32)window->Width;
	window->HeightScaled = h / (f32)window->Height;

	OPglewInit();

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

	OPRENDER_SCREEN_WIDTH = window->Width;
	OPRENDER_SCREEN_HEIGHT = window->Height;

	OPRENDER_SCREEN_WIDTH_SCALE = window->WidthScaled;
	OPRENDER_SCREEN_HEIGHT_SCALE = window->HeightScaled;
	OPRENDER_SCALED_WIDTH = OPRENDER_SCREEN_WIDTH * OPRENDER_SCREEN_WIDTH_SCALE;
	OPRENDER_SCALED_HEIGHT = OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE;

	OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
	OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;

	OPrenderSetViewport(0, 0, (OPuint)OPRENDER_SCREEN_WIDTH, (OPuint)OPRENDER_SCREEN_HEIGHT);

	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE = window;
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
	return OPvec2(x, y);
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

void OPwindowGLDestroy(OPwindow* window) {
	OPwindowGL* windowGL = (OPwindowGL*)window->internalPtr;
	OPwindowGLUnbind(window);
	glfwDestroyWindow(windowGL->Handle);
	OPfree(window);
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
	window->GetCursorPos = OPwindowGLGetCursorPos;
	window->GetButtonState = OPwindowGLGetButtonState;
	window->Unbind = OPwindowGLUnbind;
	window->Destroy = OPwindowGLDestroy;

	for (ui32 i = 0; i < (ui32)OPmouseButton::MAX; i++) {
		OPmouseMappingGL[i] = i;
	}
	for (ui32 i = 0; i < (ui32)OPkeyboardKey::MAX; i++) {
		OPkeyboardMappingGL[i] = i;
	}
}