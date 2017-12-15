#pragma once

struct OPtouchState;
typedef struct OPtouchState OPtouchState;

#include "./Core/include/OPtypes.h"

struct OPtouch {
	OPint tapping;
	OPint prevTapping;
	OPint updated;
	f32	positionX;
	f32	positionY;
	f32	prevPositionX;
	f32	prevPositionY;
	void Update();
	
	static void Init();

	inline f32 PositionX() {
		return positionX;
	}
	
	inline f32 PositionY() {
		return positionY;
	}
	
	inline f32 PositionMovedX() {
		return prevPositionX - positionX;
	}
	
	inline f32 PositionMovedY() {
		return prevPositionY - positionY;
	}
	
	inline OPint IsDown() {
		return tapping;
	}
	
	inline OPint IsUp() {
		return !tapping;
	}
	
	inline OPint WasPressed() {
		return tapping && !prevTapping;
	}
	
	inline OPint WasReleased() {
		return !tapping && prevTapping;
	}

	inline OPint AnyInputIsDown() {
		return tapping;
	}
};

extern OPtouch OPTOUCH;
