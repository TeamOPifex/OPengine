#pragma once

#ifdef OPIFEX_ANDROID

enum OUYAGamePadAxes
{
    OUYA_AXIS_L2 = 17,
    OUYA_AXIS_LS_X = 0,
    OUYA_AXIS_LS_Y = 1,
    OUYA_AXIS_R2 = 18,
    OUYA_AXIS_RS_X = 11,
    OUYA_AXIS_RS_Y = 14,
};

#endif

enum struct OPgamePadAxis
{
	LS_X = 0,
	LS_Y = 1,
	RS_X = 2,
	RS_Y = 3,
	L2 = 4,
	R2 = 5,
	_MAX = 6
};