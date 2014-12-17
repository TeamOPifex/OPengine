#ifndef OPENGINE_HUMAN_INPUT_ENUMS_GAMEPADBUTTONS
#define OPENGINE_HUMAN_INPUT_ENUMS_GAMEPADBUTTONS

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

typedef enum
{
	GamePad_Button_DPAD_UP          = 0,
	GamePad_Button_DPAD_DOWN        = 1,
	GamePad_Button_DPAD_LEFT        = 2,
	GamePad_Button_DPAD_RIGHT       = 3,
	GamePad_Button_START            = 4,
	GamePad_Button_BACK             = 5,
	GamePad_Button_LEFT_THUMB       = 6,
	GamePad_Button_RIGHT_THUMB      = 7,
	GamePad_Button_LEFT_SHOULDER    = 8,
	GamePad_Button_RIGHT_SHOULDER   = 9,
	GamePad_Button_A                = 10,
	GamePad_Button_B                = 11,
	GamePad_Button_X                = 12,
	GamePad_Button_Y                = 13,
	GamePadButton_Max				= 14
} OPgamePadButtons;

#endif