#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPglew.h"
#include <windows.h>
#include <windowsx.h>

OPwindow* OPWINDOW_ACTIVE = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

void OPwindow::Init(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height) {
	ASSERT(fullscreen == false || (fullscreen && monitor != NULL), "To create a fullscreen window, a monitor must be declared");

#if defined(OPIFEX_OPENGL_2_0)// || defined(OPIFEX_OPENGL_3_3)
	GLFWmonitor* display = NULL;
	if (monitor != NULL) {
		OPlog("Not showing monitor");
		display = monitor->Monitor;
	}
	glfwWindowHint(GLFW_DECORATED, borderless);
	Window = glfwCreateWindow(width, height, title, display, NULL);

	//glfwSetCharCallback(window, glfwCharacterCallback);
	//glfwSetDropCallback(window, glfwWindowDropCallback);

	ASSERT(Window != NULL, "Unable to create the window.");

	glfwSetInputMode(Window, GLFW_STICKY_KEYS, true);

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
#else
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = OP_HINSTANCE;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, width, height };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	Window = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		title,   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		OP_HINSTANCE,    // application handle
		NULL);    // used with multiple windows, NULL

	ShowWindow(Window, true);

	OPWINDOW_ACTIVE = this;
#endif
}

OPint OPwindow::Update() {
	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}

	// return this part of the WM_QUIT message to Windows
	//return msg.wParam;
	if (msg.message == WM_QUIT)
		return 1;

	return 0;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void OPwindow::Bind() {
	if (OPWINDOW_ACTIVE == this) return;
#ifndef OPIFEX_DIRECTX_11
	glfwMakeContextCurrent(Window);
#else

#endif

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
#ifndef OPIFEX_DIRECTX_11
	glfwSetWindowPos(Window, x, y);
#endif
}

void OPwindow::Focus() {
#ifndef OPIFEX_DIRECTX_11
	glfwFocusWindow(Window);
#endif
}