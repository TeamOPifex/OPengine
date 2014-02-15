#ifdef OPIFEX_OCULUS

#ifndef OPENGINE_HUMAN_INPUT_OCULUS
#define OPENGINE_HUMAN_INPUT_OCULUS

#include "./Math/include/Vector2.h"
#include "./Math/include/Vector4.h"

#include "OVR.h"

using namespace OVR;

typedef struct {
	DeviceManager* _manager;
	HMDDevice* _device;
	SensorDevice* _sensor;
	HMDInfo* current;
	SensorFusion* _fusion;
	Profile* _profile;
} OPoculus;

extern OPoculus* OculusManager;

int OPoculusInitialize();
void OPoculusDestroy();

void OPoculusUpdate();
OPvec4 OPoculusHmd();
OPvec2 OPoculusScreenSize();
OPfloat OPoculusEyeHeight();

#endif

#endif