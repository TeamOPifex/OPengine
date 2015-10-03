

#define OVR_OS_WIN32

#ifndef OPENGINE_HUMAN_INPUT_OCULUS
#define OPENGINE_HUMAN_INPUT_OCULUS

#ifdef OPIFEX_OPTION_OCULUS
#include "./Math/include/Vector2.h"
#include "./Math/include/Vector4.h"

#include "./Human/include/Rendering/FrameBuffer.h"

#include "./External/OculusDK2/src/OVR_CAPI.h"
#include "./External/OculusDK2/src/OVR_CAPI_GL.h"
#endif

struct OPoculus {
#ifdef OPIFEX_OPTION_OCULUS
	ovrHmd _hmdDevice;
	OPframeBuffer _frameBuffer;
	ovrFrameTiming _timing;
	ovrGLTexture* _ovrTexture;
	ovrEyeRenderDesc _eyeRenderDesc;
#endif
};
typedef struct OPoculus OPoculus;

extern OPoculus* OculusManager;

int OPoculusStartup();
int OPoculusInitialize();
void OPoculusDestroy();

void OPoculusUpdate();


void OPoculusBegin();
void OPoculusEnd();

#endif
