#pragma once

#include "./Core/include/Types.h"
#include "GamePadEnums.h"
#include "./Human/include/Math/Vector2.h"

class GamePadState 
{
public:	
	bool IsDown(GamePadButton button);
	bool IsUp(GamePadButton button);	
	bool WasPressed(GamePadButton button);
	bool WasReleased(GamePadButton button);

	Vector2 LeftThumb();
	f32 LeftThumbX();
	f32 LeftThumbY();
	f32 RightThumbX();
	f32 RightThumbY();
	f32 LeftTrigger();
	f32 RightTrigger();

	void SetButton(GamePadButton button, bool down);
	void SetAxis(GamePadAxes axis, f32 val);
		
	void SetLeftThumb(Vector2* val);
	void SetLeftThumb(f32 x, f32 y);
	void SetLeftThumbX(f32 val);
	void SetLeftThumbY(f32 val);
	void SetRightThumb(Vector2* val);
	void SetRightThumb(f32 x, f32 y);
	void SetRightThumbX(f32 val);
	void SetRightThumbY(f32 val);
	void SetLeftTrigger(f32 val);
	void SetRightTrigger(f32 val);

	bool IsConnected();
	void SetConnected(bool connected);

	void SetDeadzone(f32 deadzone);

	void Update();
	void Reset();

private:
	bool		_buttons[GamePadButton_Max];
	bool		_prevButtons[GamePadButton_Max];
	f32			_axes[GamePadAxes_Max];
	bool		_connected;
	f32			_deadzone;
};