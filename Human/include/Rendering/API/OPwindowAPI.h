#pragma once

struct OPwindowAPI;
typedef struct OPwindowAPI OPwindowAPI;

#include "./Human/include/Input/Enums/OPmouseButtons.h"
#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Human/include/Rendering/OPwindowParameters.h"
#include "./Math/include/OPvec2.h"

struct OPwindow;
struct OPmonitor;

struct OPwindowAPI {
	OPwindow*(*_Create)(OPmonitor* monitor, OPwindowParameters windowParameters) = 0;
	OPwindow*(*_Init)(OPwindow* window, OPmonitor* monitor, OPwindowParameters windowParameters) = 0;
	OPwindowParameters(*DefaultParameters)() = 0;
	void(*SetPosition)(OPwindow* window, i32 x, i32 y);
	void(*Focus)(OPwindow* window);
	void(*Bind)(OPwindow* window) = 0;
	OPint(*Update)(OPwindow* window) = 0;
	OPvec2(*GetCursorPos)(OPwindow* window) = 0;
	void(*SetCursorPos)(OPwindow* window, OPvec2 pos) = 0;
	bool(*GetButtonState)(OPwindow* window, OPmouseButton mouseButton) = 0;
	bool(*GetKeyboardState)(OPwindow* window, OPkeyboardKey keyboardkey) = 0;
	void(*Unbind)(OPwindow* window) = 0;
	void(*Destroy)(OPwindow* window) = 0;

	inline OPwindow* Create(OPmonitor* monitor, OPwindowParameters windowParameters) {
		return _Create(monitor, windowParameters);
	}

	inline OPwindow* Create(OPmonitor* monitor) {
		return _Create(monitor, DefaultParameters());
	}

	inline OPwindow* Create() {
		return _Create(NULL, DefaultParameters());
	}

	inline OPwindow* Init(OPwindow* window, OPmonitor* monitor, OPwindowParameters windowParameters) {
		return _Init(window, monitor, windowParameters);
	}

	inline OPwindow* Init(OPwindow* window, OPmonitor* monitor) {
		return _Init(window, monitor, DefaultParameters());
	}

	inline OPwindow* Init(OPwindow* window) {
		return _Init(window, NULL, DefaultParameters());
	}
};