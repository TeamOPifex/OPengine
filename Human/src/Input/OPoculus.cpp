#include "./Human/include/Input/OPoculus.h"

#ifdef OPIFEX_OCULUS
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

//#include "./External/glfw-3.0.4/include/GLFW/glfw3native.h"

OPoculus* OculusManager = NULL;
i32 oculusFrameIndex = 0;
#endif

void OPoculusUpdate() {
#ifdef OPIFEX_OCULUS
	ASSERT(OculusManager != NULL, "Oculus hasn't been initialized");
	ovrHSWDisplayState hswDisplayState;
	ovrHmd_GetHSWDisplayState(OculusManager->_hmdDevice, &hswDisplayState);

	if (hswDisplayState.Displayed) {
		ovrHmd_DismissHSWDisplay(OculusManager->_hmdDevice);
	}

	ovrTrackingState ts = ovrHmd_GetTrackingState(OculusManager->_hmdDevice, ovr_GetTimeInSeconds());

	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
		ovrPoseStatef pose = ts.HeadPose;
	}
#endif
}

void OPoculusBegin() {
#ifdef OPIFEX_OCULUS
	OculusManager->_timing = ovrHmd_BeginFrame(OculusManager->_hmdDevice, oculusFrameIndex++);
#endif
}

void OPoculusEnd() {
#ifdef OPIFEX_OCULUS

	ovrEyeType eye0 = OculusManager->_hmdDevice->EyeRenderOrder[0];
	ovrEyeType eye1 = OculusManager->_hmdDevice->EyeRenderOrder[1];

	ovrPosef headPose[2];
	headPose[0] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye0);
	headPose[1] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye1);

	ovrHmd_EndFrame(OculusManager->_hmdDevice, headPose, &OculusManager->_ovrTexture[0].Texture);
#endif
}

int OPoculusStartup() {
#ifdef OPIFEX_OCULUS
	ASSERT(OculusManager == NULL, "Oculus has already been initialized");
	if (OculusManager != NULL) {
		return 0;
	}

	if (!ovr_Initialize()) {
		return 0;
	}

	OPlog("Oculus Successfully Initialized");

	OculusManager = (OPoculus*)OPalloc(sizeof(OPoculus));

	i32 devices = ovrHmd_Detect();
	OPlog("HMD Devices Found: %d", devices);

	OculusManager->_hmdDevice = ovrHmd_Create(0);
	if (!OculusManager->_hmdDevice) {
		return 0;
	}
#endif
	return 1;
}

int OPoculusInitialize() {
#ifdef OPIFEX_OCULUS
	ASSERT(OculusManager != NULL, "Oculus has already been initialized");

	ovrSizei resolution = OculusManager->_hmdDevice->Resolution;

	ovrHmd_ConfigureTracking(OculusManager->_hmdDevice,
		ovrTrackingCap_Orientation |
		ovrTrackingCap_MagYawCorrection |
		ovrTrackingCap_Position,
		0
		);


	ovrSizei tex0Size = ovrHmd_GetFovTextureSize(OculusManager->_hmdDevice, ovrEye_Left, OculusManager->_hmdDevice->DefaultEyeFov[0], 1.0f);
	ovrSizei tex1Size = ovrHmd_GetFovTextureSize(OculusManager->_hmdDevice, ovrEye_Right, OculusManager->_hmdDevice->DefaultEyeFov[1], 1.0f);
	ovrSizei renderTargetSize;
	ui32 w = tex0Size.w + tex1Size.w;
	ui32 h = tex0Size.h + tex1Size.h;
	const i32 eyeRenderMultisample = 1;

	OPtextureDescription desc = {
		w, h,
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		OPtextureLinear, OPtextureLinear,
		OPtextureClamp, OPtextureClamp
	};


	renderTargetSize.w = w;
	renderTargetSize.h = h;

	//ovrPosef headPose[2];

	ovrEyeType eye0 = OculusManager->_hmdDevice->EyeRenderOrder[0];
	ovrEyeType eye1 = OculusManager->_hmdDevice->EyeRenderOrder[1];

	//headPose[0] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye0);
	//headPose[1] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye1);
	
	ovrRecti frameSize;
	frameSize.Pos.x = 0;
	frameSize.Pos.y = 0;
	frameSize.Size.w = w / 2.0;
	frameSize.Size.h = h;

	OculusManager->_frameBuffer = OPframeBufferCreate(desc);
	OculusManager->_ovrTexture = (ovrGLTexture*)OPalloc(sizeof(ovrGLTexture)* 2);
	OculusManager->_ovrTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	OculusManager->_ovrTexture[0].OGL.Header.TextureSize = renderTargetSize;
	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport = frameSize;
	OculusManager->_ovrTexture[0].OGL.TexId = OculusManager->_frameBuffer.Handle;		
	frameSize.Pos.x = w / 2.0;
	OculusManager->_ovrTexture[1].OGL.Header.API = ovrRenderAPI_OpenGL;
	OculusManager->_ovrTexture[1].OGL.Header.TextureSize = renderTargetSize;
	OculusManager->_ovrTexture[1].OGL.Header.RenderViewport = frameSize;
	OculusManager->_ovrTexture[1].OGL.TexId = OculusManager->_frameBuffer.Handle;

	ovrGLConfig cfg;
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	cfg.OGL.Header.RTSize = renderTargetSize;
	cfg.OGL.Header.Multisample = eyeRenderMultisample;
	//cfg.OGL.Window = glfwGetWin32Window(window);
	//cfg.OGL.DC = (HDC)glfwGetWGLContext(window);
		
	ovrBool result = ovrHmd_ConfigureRendering(OculusManager->_hmdDevice, &cfg.Config,
		ovrDistortionCap_Chromatic |
		ovrDistortionCap_TimeWarp |
		ovrDistortionCap_Overdrive,
		OculusManager->_hmdDevice->DefaultEyeFov, &OculusManager->_eyeRenderDesc);

	//ovrHmd_AttachToWindow(OculusManager->_hmdDevice, glfwGetWin32Window(window), NULL, NULL);

#endif
	return 1;
}

void OPoculusDestroy() {
#ifdef OPIFEX_OCULUS
	if (OculusManager == NULL) return;

	ovrHmd_Destroy(OculusManager->_hmdDevice);
	OPfree(OculusManager);

	ovr_Shutdown();
#endif
}