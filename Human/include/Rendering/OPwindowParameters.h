#ifndef OPENGINE_HUMAN_RENDERING_WINDOW_PARAMETERS
#define OPENGINE_HUMAN_RENDERING_WINDOW_PARAMETERS

struct OPwindowParameters;
typedef struct OPwindowParameters OPwindowParameters;

#include "./Core/include/OPtypes.h"

struct OPwindowParameters {
	const OPchar* title;
	ui32 width;
	ui32 height;
	bool borderless;

	OPwindowParameters() { }
	OPwindowParameters(const OPchar* title, bool borderless) {
		this->title = title;
		this->borderless = borderless;
		this->width = 1280;
		this->height = 720;
	}
	OPwindowParameters(const OPchar* title, bool borderless, ui32 width, ui32 height) {
		this->title = title;
		this->borderless = borderless;
		this->width = width;
		this->height = height;
	}
};

#endif