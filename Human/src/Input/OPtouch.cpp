#include "./Human/include/Input/OPtouch.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

OPtouch OPTOUCH = {
	false,
	false,
	false,
	0.0,
	0.0,
	0.0,
	0.0
};

#ifndef OPIFEX_ANDROID

void OPtouch::Init() {
	
}

void OPtouch::Update() {

}

#else

#include "./Core/include/Platform/Android.h"

f32 lastKnownTouchX = 0;
f32 lastKnownTouchY = 0;
bool lastKnownTapping = false;

/**
 * Process the next input event.
 */
 static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

		
		switch(AInputEvent_getSource(event)){
			case AINPUT_SOURCE_TOUCHSCREEN:
                OPfloat x = AMotionEvent_getX(event, 0);
                OPfloat y = AMotionEvent_getY(event, 0);
				lastKnownTouchX = x;
				lastKnownTouchY = y;
				int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
				switch(action){
					case AMOTION_EVENT_ACTION_DOWN:
					lastKnownTapping = true;
					OPTOUCH.positionX = x;
					OPTOUCH.positionY = y;
					break;
					case AMOTION_EVENT_ACTION_UP:
					lastKnownTapping = false;
					break;
					case AMOTION_EVENT_ACTION_MOVE:
					
					break;
				}
			break;
		} // end switch
		return 1;
	}

	return 0;
}

void OPtouch::Init() {
    OPAndroidState->onInputEvent = engine_handle_input;
}
 

void OPtouch::Update() {
    prevPositionX = positionX;
    prevPositionY = positionY;
	positionX = lastKnownTouchX;
	positionY = lastKnownTouchY;
	prevTapping = tapping;
	tapping = lastKnownTapping;
    updated = false;
}

#endif