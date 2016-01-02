#include "./Human/include/Input/OPoculus.h"

#ifdef OPIFEX_OPTION_OCULUS
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

//#include "./External/glfw-3.0.4/include/GLFW/glfw3native.h"

OPoculus OculusManager;
i8 OculusInitialized = 0;
i32 oculusFrameIndex = 0;
#endif

void OPoculusUpdate() {
#ifdef OPIFEX_OPTION_OCULUS
	ASSERT(OculusInitialized != 0, "Oculus hasn't been initialized");

	//ovrHSWDisplayState hswDisplayState;
	//ovrHmd_GetHSWDisplayState(OculusManager->_hmdDevice, &hswDisplayState);

	//if (hswDisplayState.Displayed) {
	//	ovrHmd_DismissHSWDisplay(OculusManager->_hmdDevice);
	//}

	ovrTrackingState ts = ovr_GetTrackingState(OculusManager.session, ovr_GetTimeInSeconds(), false);
	//ovrTrackingState ts = ovrHmd_GetTrackingState(OculusManager->_hmdDevice, ovr_GetTimeInSeconds());

	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
		ovrPoseStatef pose = ts.HeadPose;
	}
#endif
}

void OPoculusBegin() {
#ifdef OPIFEX_OPTION_OCULUS
	//OculusManager->_timing = ovrHmd_BeginFrame(OculusManager->_hmdDevice, oculusFrameIndex++);
#endif
}

void OPoculusEnd() {
#ifdef OPIFEX_OPTION_OCULUS

/*	ovrEyeType eye0 = OculusManager->_hmdDevice->EyeRenderOrder[0];
	ovrEyeType eye1 = OculusManager->_hmdDevice->EyeRenderOrder[1];

	ovrPosef headPose[2];
	headPose[0] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye0);
	headPose[1] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye1);

	ovrHmd_EndFrame(OculusManager->_hmdDevice, headPose, &OculusManager->_ovrTexture[0].Texture);*/

#endif
}

int OPoculusStartup() {
#ifdef OPIFEX_OPTION_OCULUS
	ASSERT(OculusInitialized == 0, "Oculus has already been initialized");

	ovrResult result = ovr_Initialize(nullptr);
	if (OVR_FAILURE(result)) {
		OPlogErr("Failed to Initialize Oculus");
		return -1;
	}

	OPlog("Oculus Successfully Initialized");

	result = ovr_Create(&OculusManager.session, &OculusManager.luid);
	if (OVR_FAILURE(result))
	{
		OPlogErr("Failed to Create Oculus");
		ovr_Shutdown();
		return -2;
	}

	OculusManager.desc = ovr_GetHmdDesc(OculusManager.session);

	OculusInitialized = 1;

#endif
	return 1;
}

int OPoculusInitialize() {
#ifdef OPIFEX_OPTION_OCULUS
	ASSERT(OculusInitialized == 0, "Oculus has already been initialized");

	ovrSizei resolution = OculusManager.desc.Resolution;



	//ovrHmd_ConfigureTracking(OculusManager->_hmdDevice,
	//	ovrTrackingCap_Orientation |
	//	ovrTrackingCap_MagYawCorrection |
	//	ovrTrackingCap_Position,
	//	0
	//	);

	ovrSizei tex0Size = ovr_GetFovTextureSize(OculusManager.session, ovrEye_Left, OculusManager.desc.DefaultEyeFov[0], 1.0f);
	ovrSizei tex1Size = ovr_GetFovTextureSize(OculusManager.session, ovrEye_Right, OculusManager.desc.DefaultEyeFov[1], 1.0f);
	ovrSizei renderTargetSize;
	ui32 w = tex0Size.w + tex1Size.w;
	ui32 h = OPMAX(tex0Size.h, tex1Size.h);
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

	ovrEyeRenderDesc EyeRenderDesc[2];
	EyeRenderDesc[0] = ovr_GetRenderDesc(OculusManager.session, ovrEye_Left, OculusManager.desc.DefaultEyeFov[0]);
	EyeRenderDesc[1] = ovr_GetRenderDesc(OculusManager.session, ovrEye_Right, OculusManager.desc.DefaultEyeFov[1]);


	//headPose[0] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye0);
	//headPose[1] = ovrHmd_GetEyePose(OculusManager->_hmdDevice, eye1);
	
	ovrRecti frameSize;
	frameSize.Pos.x = 0;
	frameSize.Pos.y = 0;
	frameSize.Size.w = w / 2.0;
	frameSize.Size.h = h;

	OculusManager.frameBuffer = OPframeBufferCreate(desc);
	OculusManager.ovrTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	OculusManager.ovrTexture[0].OGL.Header.TextureSize = renderTargetSize;
	//OculusManager.ovrTexture[0].OGL.Header.RenderViewport = frameSize;
	OculusManager.ovrTexture[0].OGL.TexId = OculusManager.frameBuffer.Handle;		
	frameSize.Pos.x = w / 2.0;
	OculusManager.ovrTexture[1].OGL.Header.API = ovrRenderAPI_OpenGL;
	OculusManager.ovrTexture[1].OGL.Header.TextureSize = renderTargetSize;
	//OculusManager.ovrTexture[1].OGL.Header.RenderViewport = frameSize;
	OculusManager.ovrTexture[1].OGL.TexId = OculusManager.frameBuffer.Handle;

	// Make sure VSYNC is disabled
	glfwSwapInterval(0);

	//ovrGLConfig cfg;
	//cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	//cfg.OGL.Header.RTSize = renderTargetSize;
	//cfg.OGL.Header.Multisample = eyeRenderMultisample;
	//cfg.OGL.Window = glfwGetWin32Window(window);
	//cfg.OGL.DC = (HDC)glfwGetWGLContext(window);
		
	//ovrBool result = ovrHmd_ConfigureRendering(OculusManager->_hmdDevice, &cfg.Config,
	//	ovrDistortionCap_Chromatic |
	//	ovrDistortionCap_TimeWarp |
	//	ovrDistortionCap_Overdrive,
	//	OculusManager->_hmdDevice->DefaultEyeFov, &OculusManager->_eyeRenderDesc);

	//ovrHmd_AttachToWindow(OculusManager->_hmdDevice, glfwGetWin32Window(window), NULL, NULL);

#endif
	return 1;
}

void OPoculusDestroy() {
#ifdef OPIFEX_OPTION_OCULUS

	ovr_Destroy(OculusManager.session);
	ovr_Shutdown();
	OculusInitialized = 0;

#endif
}