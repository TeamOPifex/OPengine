#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPglew.h"

OPwindow* OPWINDOW_ACTIVE = NULL;

void OPwindow::Init(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height) {
	ASSERT(fullscreen == false || (fullscreen && monitor != NULL), "To create a fullscreen window, a monitor must be declared");
	GLFWmonitor* display = NULL;
	if (monitor != NULL) {
		display = monitor->Monitor;
	}
	glfwWindowHint(GLFW_DECORATED, !borderless);
	Window = glfwCreateWindow(width, height, title, display, NULL);
	
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, true);
	//glfwSetCharCallback(window, glfwCharacterCallback);
	//glfwSetDropCallback(window, glfwWindowDropCallback);

	ASSERT(Window != NULL, "Unable to create the window.");

	Width = width;
	Height = height;

	int w, h;
	glfwGetFramebufferSize(Window, &w, &h);
	WidthScaled = w / (f32)Width;
	HeightScaled = h / (f32)Height;

	glfwMakeContextCurrent(Window);
	OPWINDOW_ACTIVE = this;
	OPglewInit();
	
	OPglError("OPrenderInit error glfw setup");

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &VAO);
}


void OPwindow::Bind() {
	if (OPWINDOW_ACTIVE == this) return;
	glfwMakeContextCurrent(Window);

	OPRENDER_SCREEN_WIDTH = Width;
	OPRENDER_SCREEN_HEIGHT = Height;

	OPRENDER_SCREEN_WIDTH_SCALE = WidthScaled;
	OPRENDER_SCREEN_HEIGHT_SCALE = HeightScaled;
	OPRENDER_SCALED_WIDTH = OPRENDER_SCREEN_WIDTH * OPRENDER_SCREEN_WIDTH_SCALE;
	OPRENDER_SCALED_HEIGHT = OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE;
	
	OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
	OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;

	OPrenderSetViewport(0, 0, (OPuint)OPRENDER_SCREEN_WIDTH, (OPuint)OPRENDER_SCREEN_HEIGHT);
	
	OPRENDER_VAO = VAO;
	glBindVertexArray(OPRENDER_VAO);

	OPWINDOW_ACTIVE = this;
}

void OPwindow::SetPosition(i32 x, i32 y) {
	Bind();
	glfwSetWindowPos(Window, x, y);
}

void OPwindow::Focus() {
	glfwFocusWindow(Window);
}