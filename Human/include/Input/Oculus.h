#pragma once

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
} OPOculus;

extern OPOculus* OculusManager;

int OPoculusInitialize();
void OPoculusDestroy();

void OPoculusUpdate();
OPvec4 OPoculusHmd();
OPvec2 OPoculusScreenSize();
OPfloat OPoculusEyeHeight();