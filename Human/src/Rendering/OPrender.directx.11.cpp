
#include "./Human/include/Rendering/OPrender.h"

#if defined(OPIFEX_DIRECTX_11)

#include "./Human/include/Input/OPkeyboard.h"
#include "./Core/include/Assert.h"
#include "include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPmonitor.h"


// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context

i8 OPRENDER_INITIALIZED = 0;
ui32 OPRENDER_WIDTH = 1280;
ui32 OPRENDER_HEIGHT = 720;
ui32 OPRENDER_SCREEN_WIDTH;
ui32 OPRENDER_SCREEN_HEIGHT;
ui32 OPRENDER_SCALED_WIDTH;
ui32 OPRENDER_SCALED_HEIGHT;
f32 OPRENDER_SCREEN_WIDTH_SCALE;
f32 OPRENDER_SCREEN_HEIGHT_SCALE;
i8 OPRENDER_FULLSCREEN;
i8 OPRENDER_HAS_FOCUS;
OPuint OPRENDER_VAO = 0;

GLFWwindow* window;
ui8 glfwInitialized = 0;



void glfwErrorCallback(int error, const char* desc){
    OPlog("GLFW ERROR: %s", desc);
}

void glfwWindowFocusCallback(GLFWwindow* window, int code) {
    OPlogInfo("Focus Result: %d", code);
    OPRENDER_HAS_FOCUS = code;
}
void glfwWindowDropCallback(GLFWwindow* window, int count, const OPchar** files) {
    OPlog("Total Files: %d", count);
    for(OPint i = 0; i < count; i++) {
        OPlog("File: %s", files[i]);
    }
}
void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
    OPkeyboardKey(codepoint);
}

void(*OP_WINDOW_DROP)(int, const OPchar**) = NULL;
void OPrenderDragAndDropCB(void(*cb)(int, const OPchar**)) {
	OP_WINDOW_DROP = cb;
}

OPint OPrenderInit() {
    OPlogDebug("Initializing DIRECTX_11 Renderer");

    glfwSetErrorCallback(glfwErrorCallback);

    ASSERT(glfwInitialized == 0, "GLFW has already been initialized");
    OPint result = glfwInit();
    if(!result) {
        OPlogErr("Failed to initialize GLFW");
        return -1;
    }
    glfwInitialized = 1;


	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	for (ui8 i = 0; i < monitorCount & i < OPMONITOR_MAX; i++) {
		OPMONITOR_LIST[i].Init(monitors[i]);
	}

	OPMONITOR_COUNT = monitorCount;

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPMONITOR_PRIMARY.Init(primaryMonitor);


    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);

    OPlog("GLFW version %d.%d.%d", major, minor, rev);

	OPRENDER_INITIALIZED = 1;
    return 0;
}

D3D_DRIVER_TYPE g_driverType;
ID3D11Device* g_pd3dDevice = NULL;
ID3D11Device1*          g_pd3dDevice1 = nullptr;
D3D_FEATURE_LEVEL g_featureLevel;
ID3D11DeviceContext* g_pImmediateContext = NULL;
ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
IDXGISwapChain1*        g_pSwapChain1 = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

OPint OPrenderInitDevice(OPwindow* window) {

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(window->Window, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);



	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) {
		OPlogErr("Failed to find a directx 11 device");
		return -1;
	}

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr)) {
		OPlogErr("Failed to find a dxgi adapter");
		return -2;
	}

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, window->Window, &sd, nullptr, nullptr, &g_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window->Window;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(window->Window, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr)) {
		OPlogErr("Failed to associate the window with a swap chain");
		return -3;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr)) {
		OPlogErr("Failed to create a back buffer");
		return -4;
	}

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) {
		OPlogErr("Failed to create a back buffer 2");
		return -5;
	}

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	OPlog("Created a DirectX 11 Device");

	return 1;
}

OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height) {
	OPwindow* window = (OPwindow*)OPalloc(sizeof(OPwindow));
	window->Init(monitor, fullscreen, borderless, title, width, height);
	return window;
}

void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPvec4 val(r, g, b, a);
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, (const float*)&val);
}
void  OPrenderSetScreenSize(OPuint width, OPuint height) {
    OPRENDER_SCREEN_WIDTH = width;
    OPRENDER_SCREEN_HEIGHT = height;
}
void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height) {
	//glViewport(x, y, width, height);
    OPRENDER_WIDTH  = width;
    OPRENDER_HEIGHT = height;
}
void OPrenderResetViewport() {
    OPrenderSetViewport(0, 0, OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
}
OPint OPrenderGetWidth() {
    return OPRENDER_WIDTH;
}
OPint OPrenderGetHeight() {
    return OPRENDER_HEIGHT;
}
OPfloat OPrenderGetAspectRatio() {
    ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
    return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}
OPfloat OPrenderGetWidthAspectRatio() {
    ASSERT(OPRENDER_WIDTH > 0, "Height was not greater than 0, there was problem getting width and height");
    OPfloat aspect = OPRENDER_HEIGHT / (OPfloat)OPRENDER_WIDTH;
    return aspect > 1.0f ? 1.0f : aspect;
}
OPfloat OPrenderGetHeightAspectRatio() {
    ASSERT(OPRENDER_HEIGHT > 0, "Height was not greater than 0, there was problem getting width and height");
    OPfloat aspect = OPRENDER_WIDTH / (OPfloat)OPRENDER_HEIGHT;
    return aspect > 1.0f ? 1.0f : aspect;
}

void OPrenderCull(OPint state) {

}

void OPrenderCullMode(OPint state) {

}

void  OPrenderSwapBuffer () {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
}

void OPrenderUpdate() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");

}

void  OPrenderPresent    () {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
	g_pSwapChain->Present(0, 0);
}

void OPrenderBlend(OPint state) {

}

void  OPrenderDepth(OPint state) {

}

void OPrenderDepthWrite(OPint state) {

}

void  OPrenderShutdown   () {
}

ui32 OPgetNativeScreenWidth() {
	return 0;
}
ui32 OPgetNativeScreenHeight() {
	return 0;
}
#endif
