#pragma once

struct OPmonitor;
typedef struct OPmonitor OPmonitor;

struct OPmonitorVideoMode;
typedef struct OPmonitorVideoMode OPmonitorVideoMode;

struct OPmonitorResult;
typedef struct OPmonitorResult OPmonitorResult;

#define OPMONITOR_MAX 4

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec2.h"

struct OPmonitorVideoMode {
	i32 Width;
	i32 Height;
	i32 Red, Green, Blue;
	i32 RefreshRate;
};

struct OPmonitor {
	void* internalPtr;
	i32 X;
	i32 Y;
	i32 WidthMM;
	i32 HeightMM;

	//GLFWmonitor* Handle;
	OPmonitorVideoMode VideoModeCurrent;
	ui32 VideoModesCount;
	OPmonitorVideoMode* VideoModes;

	static OPmonitorResult GetAll();
};

struct OPmonitorResult {
	OPmonitor primary;
	OPmonitor* monitors;
	OPuint count;
};