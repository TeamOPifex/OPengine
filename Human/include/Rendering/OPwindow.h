#pragma once

struct OPwindow;
typedef struct OPwindow OPwindow;

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPwindowParameters.h"

struct OPwindow {
	void* internalPtr;
	ui32 Width;
	ui32 Height;
	ui32 WindowWidth;
	ui32 WindowHeight;
	f32 WidthScaled;
	f32 HeightScaled;

	inline void Init(OPmonitor* monitor, OPwindowParameters windowParameters) {
		OPRENDERER_ACTIVE->Window.Init(this, monitor, windowParameters);
	}

	inline void Init(OPmonitor* monitor) {
		OPRENDERER_ACTIVE->Window.Init(this, monitor);
	}

	inline void Init() {
		OPRENDERER_ACTIVE->Window.Init(this);
	}

	void SetDropCallback(void(*callback)(OPuint count, const OPchar**));

	OPint Update();
	void Bind();
	void Focus();
	void SetPosition(i32 x, i32 y);
	void Destroy();
};