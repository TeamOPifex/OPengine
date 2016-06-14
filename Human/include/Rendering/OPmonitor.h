#ifndef OPENGINE_HUMAN_RENDERING_MONITOR
#define OPENGINE_HUMAN_RENDERING_MONITOR

#define OPMONITOR_MAX 4

struct OPmonitor;
typedef struct OPmonitor OPmonitor;

struct OPmonitorVideoMode;
typedef struct OPmonitorVideoMode OPmonitorVideoMode;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OpenGL.h"
#include "./Math/include/OPvec2.h"

struct OPmonitorVideoMode {
	i32 Width;
	i32 Height;
	i32 Red, Green, Blue;
	i32 RefreshRate;
};

struct OPmonitor {
	i32 X;
	i32 Y;
	i32 WidthMM;
	i32 HeightMM;

	GLFWmonitor* Monitor;
	OPmonitorVideoMode VideoModeCurrent;
	i32 VideoModesCount;
	OPmonitorVideoMode* VideoModes;

	void Init(GLFWmonitor* monitor);
};

extern ui8 OPMONITOR_COUNT;
extern OPmonitor OPMONITOR_PRIMARY;
extern OPmonitor OPMONITOR_LIST[OPMONITOR_MAX];

#endif
