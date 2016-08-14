#pragma once

#ifdef OPIFEX_ANDROID

typedef enum
{
    OUYA_BUTTON_A = 97,
    OUYA_BUTTON_DPAD_DOWN = 20,
    OUYA_BUTTON_DPAD_LEFT = 21,
    OUYA_BUTTON_DPAD_RIGHT = 22,
    OUYA_BUTTON_DPAD_UP = 19,
    OUYA_BUTTON_L1 = 102,
    OUYA_BUTTON_L2 = 104, // Use AXIS Values Instead
    OUYA_BUTTON_L3 = 106,
    OUYA_BUTTON_MENU = 82,
    OUYA_BUTTON_O = 96,
    OUYA_BUTTON_R1 = 103,
    OUYA_BUTTON_R2 = 105, // Use AXIS Values Instead
    OUYA_BUTTON_R3 = 107,
    OUYA_BUTTON_U = 99,
    OUYA_BUTTON_Y = 100
} OUYAGamePadButton;

#endif

enum struct OPgamePadButton
{
	DPAD_UP          = 0,
	DPAD_DOWN        = 1,
	DPAD_LEFT        = 2,
	DPAD_RIGHT       = 3,
	START            = 4,
	BACK             = 5,
	LEFT_THUMB       = 6,
	RIGHT_THUMB      = 7,
	LEFT_SHOULDER    = 8,
	RIGHT_SHOULDER   = 9,
	A                = 10,
	B                = 11,
	X                = 12,
	Y                = 13,
	_MAX			 = 14
};