#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPglew.h"
#include "./Core/include/Assert.h"

#ifdef OPIFEX_DIRECTX_11
#include <windows.h>
#include <windowsx.h>
#endif

#ifdef OPIFEX_DIRECTX_11
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);
#endif
//
//void OPwindow::Init(OPmonitor* monitor, OPwindowParameters windowParameters) {
//	OPRENDERER_ACTIVE->Window.Init(this, monitor, windowParameters);
//#if defined(OPIFEX_OPENGL_2_0) || defined(OPIFEX_OPENGL_3_3)
//	//GLFWmonitor* display = NULL;
//	//if (monitor != NULL) {
//	//	OPlog("Not showing monitor");
//	//	display = monitor->Handle;
//	//}
//	//glfwWindowHint(GLFW_DECORATED, !borderless);
//	//Window = glfwCreateWindow(width, height, title, display, NULL);
//
//	////glfwSetCharCallback(window, glfwCharacterCallback);
//	////glfwSetDropCallback(window, glfwWindowDropCallback);
//
//	//ASSERT(Window != NULL, "Unable to create the window.");
//
//	//glfwSetInputMode(Window, GLFW_STICKY_KEYS, true);
//
//	//Width = width;
//	//Height = height;
//
//	//int w, h;
//	//glfwGetFramebufferSize(Window, &w, &h);
//	//WidthScaled = w / (f32)Width;
//	//HeightScaled = h / (f32)Height;
//
//	//glfwMakeContextCurrent(Window);
//	//OPWINDOW_ACTIVE = this;
//	//OPglewInit();
//
//#else
//	WNDCLASSEX wc;
//
//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = WindowProc;
//	wc.hInstance = OP_HINSTANCE;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//	wc.lpszClassName = "WindowClass1";
//
//	RegisterClassEx(&wc);
//
//	RECT wr = { 0, 0, width, height };    // set the size, but not the position
//	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
//
//	Window = CreateWindowEx(NULL,
//		"WindowClass1",    // name of the window class
//		title,   // title of the window
//		WS_OVERLAPPEDWINDOW,    // window style
//		300,    // x-position of the window
//		300,    // y-position of the window
//		wr.right - wr.left,    // width of the window
//		wr.bottom - wr.top,    // height of the window
//		NULL,    // we have no parent window, NULL
//		NULL,    // we aren't using menus, NULL
//		OP_HINSTANCE,    // application handle
//		NULL);    // used with multiple windows, NULL
//
//	ShowWindow(Window, true);
//
//	Bind();
//#endif
//}

OPint OPwindow::Update() {

#ifdef OPIFEX_DIRECTX_11
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
#else
	//glfwPollEvents();
	//if (glfwWindowShouldClose(OPWINDOW_ACTIVE->Window)) {
	//	OPend();
	//}
	return OPRENDERER_ACTIVE->Window.Update(this);
#endif
}

#ifdef OPIFEX_DIRECTX_11
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
#endif


void OPwindow::Bind() {
	OPRENDERER_ACTIVE->Window.Bind(this);
}

void OPwindow::SetPosition(i32 x, i32 y) {
//	Bind();
//#ifndef OPIFEX_DIRECTX_11
//	glfwSetWindowPos(Window, x, y);
//#endif
	OPRENDERER_ACTIVE->Window.SetPosition(this, x, y);
}

void OPwindow::Focus() {
	OPRENDERER_ACTIVE->Window.Focus(this);
//#ifndef OPIFEX_DIRECTX_11
//	glfwFocusWindow(Window);
//#endif
}




// // These are all OPwindow calls now
//
// void glfwErrorCallback(int error, const char* desc){
//     OPlog("GLFW ERROR: %s", desc);
// }
//
// void glfwWindowFocusCallback(GLFWwindow* window, int code) {
//     OPlogInfo("Focus Result: %d", code);
//     OPRENDER_HAS_FOCUS = code;
// }
// void glfwWindowDropCallback(GLFWwindow* window, int count, const OPchar** files) {
//     OPlog("Total Files: %d", count);
//     for(OPint i = 0; i < count; i++) {
//         OPlog("File: %s", files[i]);
//     }
// }
// void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint)
// {
//     OPkeyboardKey(codepoint);
// }
//
// void(*OP_WINDOW_DROP)(int, const OPchar**) = NULL;
// void OPrenderDragAndDropCB(void(*cb)(int, const OPchar**)) {
// 	OP_WINDOW_DROP = cb;
// }
