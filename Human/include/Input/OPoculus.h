

#define OVR_OS_WIN32

#ifndef OPENGINE_HUMAN_INPUT_OCULUS
#define OPENGINE_HUMAN_INPUT_OCULUS

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OCULUS
#include "./Math/include/Vector2.h"
#include "./Math/include/Vector4.h"

#include "./Human/include/Rendering/FrameBuffer.h"

#include "./External/OculusDK2/src/OVR_CAPI.h"
#include "./External/OculusDK2/src/OVR_CAPI_GL.h"
#endif

typedef struct {
#ifdef OPIFEX_OCULUS
	ovrHmd _hmdDevice;
	OPframeBuffer _frameBuffer;
	ovrFrameTiming _timing;
	ovrGLTexture* _ovrTexture;
	ovrEyeRenderDesc _eyeRenderDesc;
#endif
} OPoculus;

extern IMPORT_METHOD OPoculus* OculusManager;

EXPORT_METHOD int OPoculusStartup();
EXPORT_METHOD int OPoculusInitialize();
EXPORT_METHOD void OPoculusDestroy();

EXPORT_METHOD void OPoculusUpdate();


EXPORT_METHOD void OPoculusBegin();
EXPORT_METHOD void OPoculusEnd();

#endif