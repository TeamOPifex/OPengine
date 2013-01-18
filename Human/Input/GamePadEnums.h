#pragma once

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
} GamePadButton;

typedef enum
{
	GamePadIndex_One =  0,
	GamePadIndex_Two =  1,
	GamePadIndex_Three =2,
	GamePadIndex_Four = 3,
} GamePadIndex;

typedef enum
{
	LS_X				= 0,
	LS_Y				= 1,
	RS_X				= 2,
	RS_Y				= 3,
	L2					= 4,
	R2					= 5,
	GamePadAxes_Max		= 6
} GamePadAxes;