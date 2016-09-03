#pragma once

struct OPgamePadSystem;
typedef struct OPgamePadSystem OPgamePadSystem;

#include "./Human/include/Input/OPgamePad.h"

struct OPgamePadSystem {
	OPgamePad gamePads[(ui32)OPgamePadIndex::_MAX];

	void Update(OPtimer* timer);
	void Reset();
	void SetDeadzones(OPfloat deadzone);
	OPgamePad* Get(OPgamePadIndex index);

	inline OPgamePad* Get(i8 index) {
		ASSERT(index < (ui32)OPgamePadIndex::_MAX - 1, "Trying to access a controller that's not indexed, see CONTROLLERS global");
		return &gamePads[index];
	}

	inline OPgamePad* operator[](const i32 i) {
		return &gamePads[i];
	}

	inline bool AnyControllerInputIsDown() {
		ui32 count = (ui32)OPgamePadIndex::_MAX - 1;
		for (; count > 0; count--) {
			if (gamePads[count].AnyInputIsDown()) return true;
		}
		return false;
	}
};

extern OPgamePadSystem OPGAMEPADS;