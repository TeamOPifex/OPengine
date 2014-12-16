#ifndef OPENGINE_HUMAN_INPUT_ENUMS_GAMEPADAXES
#define OPENGINE_HUMAN_INPUT_ENUMS_GAMEPADAXES

#ifdef OPIFEX_ANDROID

typedef enum
{
    OUYA_AXIS_L2 = 17,
    OUYA_AXIS_LS_X = 0,
    OUYA_AXIS_LS_Y = 1,
    OUYA_AXIS_R2 = 18,
    OUYA_AXIS_RS_X = 11,
    OUYA_AXIS_RS_Y = 14,
} OUYAGamePadAxes;

#endif

typedef enum
{
	LS_X				= 0,
	LS_Y				= 1,
	RS_X				= 2,
	RS_Y				= 3,
	L2					= 4,
	R2					= 5,
	GamePadAxes_Max		= 6
} OPgamePadAxes;

#endif