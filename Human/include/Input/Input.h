#pragma once
// THIS CURRENTLY ONLY WORKS IN WINDOWS

#include <GL/glfw.h>

class InputSystem {

private:
	int _x;
	int _y;
	int _lastX;
	int _lastY;
	int _deltaX;
	int _deltaY;
	int _wheel;
	int _centerX;
	int _centerY;

public:
	InputSystem(int width, int height){
		_centerX = width;
		_centerY = height;
		Update();
	}

	void Update(){
		_lastX = _x;
		_lastY = _y;
		GetMousePos(&_x, &_y);
		SetMousePos(_centerX, _centerY);
		_deltaX = _x - _centerX;
		_deltaY = _y - _centerY;
		_wheel = GetMouseWheel();
	}

	void GetMouseDelta(int* x, int* y){
		(*x) = _deltaX;
		(*y) = _deltaY;
	}

	void SetMousePos(int x, int y){	
		glfwSetMousePos(x, y);
	}

	void GetMousePos(int* x, int* y){
		glfwGetMousePos(x, y);
	}

	int GetMouseWheel(){
		return glfwGetMouseWheel();
	}

	int GetMouseState(int button){
		return glfwGetMouseButton(button);
	}

	int GetKeyState(int key){
		return glfwGetKey(key);
	}
};
