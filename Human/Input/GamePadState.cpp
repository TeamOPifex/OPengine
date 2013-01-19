#include "GamePadState.h"

bool GamePadState::IsDown(GamePadButton button){
	return _buttons[button];
}

bool GamePadState::IsUp(GamePadButton button){
	return !_buttons[button];
}
	
bool GamePadState::WasPressed(GamePadButton button){
	return !_buttons[button] && _prevButtons[button];
}

bool GamePadState::WasReleased(GamePadButton button){
	return _buttons[button] && !_prevButtons[button];
}
	
Vector2 GamePadState::LeftThumb() { return Vector2(_axes[LS_X], _axes[LS_Y]); }
f32 GamePadState::LeftThumbX() { return _axes[LS_X]; }
f32 GamePadState::LeftThumbY() { return _axes[LS_Y]; }
f32 GamePadState::RightThumbX() { return _axes[RS_X]; }
f32 GamePadState::RightThumbY() { return _axes[RS_Y]; }
f32 GamePadState::LeftTrigger() { return _axes[L2]; }
f32 GamePadState::RightTrigger() { return _axes[R2]; }

void GamePadState::SetButton(GamePadButton button, bool down){
	_buttons[button] = down;
}

void GamePadState::SetAxis(GamePadAxes axis, f32 val){
	if(val < _deadzone) 
		val = 0;
	_axes[axis] = val;
}

void GamePadState::SetLeftThumb(Vector2* val) { SetLeftThumb(val->_x, val->_y); }
void GamePadState::SetLeftThumb(f32 x, f32 y) { SetLeftThumbX(x); SetRightThumbY(y); }
void GamePadState::SetLeftThumbX(f32 val) { SetAxis(LS_X, val); }
void GamePadState::SetLeftThumbY(f32 val) { SetAxis(LS_Y, val); }
void GamePadState::SetRightThumb(Vector2* val) { SetRightThumb(val->_x, val->_y); }
void GamePadState::SetRightThumb(f32 x, f32 y) { SetRightThumbX(x); SetRightThumbY(y);}
void GamePadState::SetRightThumbX(f32 val) { SetAxis(RS_X, val); }
void GamePadState::SetRightThumbY(f32 val) { SetAxis(RS_Y, val); }
void GamePadState::SetLeftTrigger(f32 val) { SetAxis(L2, val); }
void GamePadState::SetRightTrigger(f32 val) { SetAxis(R2, val); }

void GamePadState::Update(){
	for(int i = 0; i < GamePadButton_Max; i++){
		_prevButtons[i] = _buttons[i];
	}
}

void GamePadState::Reset()
{
	for (int i=0;i<(int)GamePadButton_Max;++i) _buttons[i] = false;
	for (int i=0;i<(int)GamePadButton_Max;++i) _prevButtons[i] = false;
	for (int i=0;i<(int)GamePadAxes_Max;++i) _axes[i] = false;
}

bool GamePadState::IsConnected(){
	return _connected;
}
void GamePadState::SetConnected(bool connected){
	_connected = connected;
}

void GamePadState::SetDeadzone(f32 deadzone){
	_deadzone = deadzone;
}