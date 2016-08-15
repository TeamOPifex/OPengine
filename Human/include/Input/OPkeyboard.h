#pragma once

struct OPkeyboard;
typedef struct OPkeyboard OPkeyboard;

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

extern OPkeyboard OPKEYBOARD;
extern void(*OPKEYBOARD_STREAM)(OPchar);

struct OPkeyboard {
	bool	keys[(ui32)OPkeyboardKey::_MAX];
	bool	prevKeys[(ui32)OPkeyboardKey::_MAX];

	void Update(OPtimer* timer);
	bool AnyInputIsDown();
	void UpdatePost(OPtimer* timer);

	inline bool IsDown(OPkeyboardKey key) {
		return keys[(ui32)key];
	}

	inline bool IsUp(OPkeyboardKey key) {
		return !keys[(ui32)key];
	}

	inline bool WasPressed(OPkeyboardKey key) {
		return keys[(ui32)key] && !prevKeys[(ui32)key];
	}

	inline bool WasReleased(OPkeyboardKey key) {
		return !keys[(ui32)key] && prevKeys[(ui32)key];
	}
};