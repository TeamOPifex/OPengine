#pragma once

struct OPmouse;
typedef struct OPmouse OPmouse;

extern OPmouse OPMOUSE;

#include "./Human/include/Input/Enums/OPmouseButtons.h"
#include "./Core/include/OPtypes.h"

struct OPmouse {
	i32	updatedWheel;
	i32	wheel;
	i32	prevWheel;
	d64	positionX;
	d64	positionY;
	d64	prevPositionX;
	d64	prevPositionY;
	bool keys[(ui32)OPmouseButton::_MAX];
	bool prevKeys[(ui32)OPmouseButton::_MAX];

	void Update();
	void SetPosition(i32 x, i32 y);
	void SetPositionScreenCenter();
	bool AnyInputIsDown();

	inline i32 X() {
		return (i32)positionX;
	}

	inline i32 Y() {
		return (i32)positionY;
	}

	inline i32 MovedX() {
		return (i32)(prevPositionX - positionX);
	}

	inline i32 MovedY() {
		return (i32)(prevPositionY - positionY);
	}

	inline i32 Wheel() {
		return wheel;
	}

	inline i32 WheelMoved() {
		return prevWheel - wheel;
	}

	inline bool IsDown(OPmouseButton key) {
		return keys[(ui32)key];
	}

	inline bool IsUp(OPmouseButton key) {
		return !keys[(ui32)key];
	}

	inline bool WasPressed(OPmouseButton key) {
		return keys[(ui32)key] && !prevKeys[(ui32)key];
	}

	inline bool WasReleased(OPmouseButton key) {
		return !keys[(ui32)key] && prevKeys[(ui32)key];
	}
};

