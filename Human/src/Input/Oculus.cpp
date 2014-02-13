#include "./Human/include/Input/Oculus.h"

#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Core/include/Assert.h"

#include <Kernel/OVR_SysFile.h>
#include <Kernel/OVR_Log.h>
#include <Kernel/OVR_Timer.h>

OPoculus* OculusManager = NULL;

static void getDk1HmdValues(OVR::HMDInfo* hmdInfo) {
	hmdInfo->HResolution = 1280;
	hmdInfo->VResolution = 800;
	hmdInfo->HScreenSize = 0.14976f;
	hmdInfo->VScreenSize = 0.09360f;
	hmdInfo->VScreenCenter = 0.04680f;
	hmdInfo->EyeToScreenDistance = 0.04100f;
	hmdInfo->LensSeparationDistance = 0.06350f;
	hmdInfo->InterpupillaryDistance = 0.06400f;
	hmdInfo->DistortionK[0] = 1;
	hmdInfo->DistortionK[1] = 0.22f;
	hmdInfo->DistortionK[2] = 0.24f;
	hmdInfo->DistortionK[3] = 0;
	hmdInfo->DesktopX = 100;
	hmdInfo->DesktopY = 100;
	hmdInfo->ChromaAbCorrection[0] = 0.99600f;
	hmdInfo->ChromaAbCorrection[1] = -0.00400f;
	hmdInfo->ChromaAbCorrection[2] = 1.01400f;
	hmdInfo->ChromaAbCorrection[3] = 0;
}

void OPoculusUpdate() {
	bool result = OculusManager->_device->GetDeviceInfo(OculusManager->current);
	if (!result) {
		getDk1HmdValues(OculusManager->current);
	}
}

int OPoculusInitialize() {
	ASSERT(OculusManager == NULL, "Oculus has already been initialized");
	if (OculusManager != NULL) {
		return 0;
	}

	OPoculus* oculus = (OPoculus*)OPalloc(sizeof(OPoculus));
	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	oculus->_manager = DeviceManager::Create();
	if (oculus->_manager == NULL) return -1;

	oculus->_device = oculus->_manager->EnumerateDevices<HMDDevice>().CreateDevice();
	if (oculus->_device == NULL) return -1;

	oculus->_sensor = oculus->_device->GetSensor();
	if (oculus->_sensor == NULL) return -1;

	oculus->_fusion = new SensorFusion();
	oculus->_fusion->AttachToSensor(oculus->_sensor);

	oculus->current = new HMDInfo();

	oculus->_profile = oculus->_device->GetProfile();

	OculusManager = oculus;

	return 1;
}

void OPoculusDestroy() {
	if (OculusManager == NULL) return;

	delete OculusManager->current;
	delete OculusManager->_sensor;
	OPfree(OculusManager);
	OVR::System::Destroy();
}

OPvec4 OPoculusHmd() {
	ASSERT(OculusManager != NULL, "Oculus has not been initialized yet");
	return OPvec4Create(
		OculusManager->current->DistortionK[0],
		OculusManager->current->DistortionK[1],
		OculusManager->current->DistortionK[2],
		OculusManager->current->DistortionK[3]);
}

OPvec2 OPoculusScreenSize() {
	ASSERT(OculusManager != NULL, "Oculus has not been initialized yet");
	return OPvec2Create(OculusManager->current->HResolution, OculusManager->current->VResolution);
}

OPfloat OPoculusEyeHeight() {
	ASSERT(OculusManager != NULL, "Oculus has not been initialized yet");
	if (OculusManager->_profile) {
		return OculusManager->_profile->GetEyeHeight();
	}
}