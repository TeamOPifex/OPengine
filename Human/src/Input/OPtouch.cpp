#include "./Human/include/Input/OPtouch.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/OPlog.h"

#ifndef OPIFEX_ANDROID
	#include <GLFW/glfw3.h>
#endif


OPtouchState Touch = {
	false,
	false,
	false,
	0.0,
	0.0,
	0.0,
	0.0
};



#ifndef OPIFEX_ANDROID

void OPtouchUpdate() {
}

f32 OPtouchPositionX() {
	return Touch.positionX;
}

f32 OPtouchPositionY() {
	return 0;
}

f32 OPtouchPositionMovedX() {
	return 0;
}

f32 OPtouchPositionMovedY() {
	return 0;
}

OPint OPtouchIsDown() {
	return false;
}

OPint OPtouchIsUp() {
	return true;
}

OPint OPtouchWasPressed() {
	return false;
}

OPint OPtouchWasReleased() {
	return false;
}

OPint OPtouchAnyInputIsDown() {
	return false;
}
#else

#include <android/sensor.h>
ASensorManager* sensorManager;
const ASensor* accelerometerSensor;

void SetupSensorManager() {
	sensorManager = ASensorManager_getInstance();
	accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
}

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_touch(JNIEnv * env, jobject obj,  jint evt, jfloat x, jfloat y){
	Touch.updated = true;
	if(evt == 1) {
		Touch.tapping = true;
	} else {
		Touch.tapping = false;
	}
	Touch.positionX = x;
	Touch.positionY = y;
	OPlog("Event! %d %f %f : Prev %f %f : Diff: %f %f", evt, Touch.positionX, Touch.positionY, Touch.prevPositionX, Touch.prevPositionY, OPtouchPositionMovedX(), OPtouchPositionMovedY());
}


void OPtouchUpdate() {
	if(Touch.updated) {
		Touch.prevPositionX = Touch.positionX;
		Touch.prevPositionY = Touch.positionY;
		Touch.prevTapping = Touch.tapping;
		Touch.updated = false;
	}
}

f32 OPtouchPositionX() {
	return Touch.positionX;
}

f32 OPtouchPositionY() {
	return Touch.positionY;
}

f32 OPtouchPositionMovedX() {
	return Touch.prevPositionX - Touch.positionX;
}

f32 OPtouchPositionMovedY() {
	return Touch.prevPositionY - Touch.positionY;
}

OPint OPtouchIsDown() {
	return Touch.tapping;
}

OPint OPtouchIsUp() {
	return !Touch.tapping;
}

OPint OPtouchWasPressed() {
	return Touch.tapping && !Touch.prevTapping;
}

OPint OPtouchWasReleased() {
	return !Touch.tapping && Touch.prevTapping;
}
OPint OPtouchAnyInputIsDown() {
	return Touch.tapping;
}
#endif