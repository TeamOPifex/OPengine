// Camera.cpp
#include "Camera.h"
#include "./Core/include/Log.h"

Camera Camera::GameCamera = Camera(
	Vector3(0, 0, 5), // position at origin
	Vector3(0, 0, 0), // looking down the Z-axis
	Vector3(0, 1, 0), // up
	-100.0f,
	10000.0f,
	-150.0f,
	150.0f,
	75.0f,
	-75.0f
);

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect){
	_viewStale = _projStale = false;
	_pos = position;
	_targ = target;
	_up = up;
	_fov = fov;

	// construct matricies
	_proj = Matrix4::CreatePerspective(
		_fov,
		_aspect = aspect,
		_near = near,
		_far = far
	);

	_view = Matrix4::CreateLook(
		_pos,
		_targ,
		_up
	);
}

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top){
	_viewStale = _projStale = false;
	_pos = position;
	_targ = target;
	_up = up;
	_fov = 0;

	// construct matricies
	_proj = Matrix4::CreateOrthographic(
		left, right,
		bottom, top,
		_near = near,
		_far = far
	);

	_view = Matrix4::CreateLook(
		_pos,
		_targ,
		_up
	);
}

void Camera::SetPosition(Vector3 position){
	_viewStale = true;
	_pos = position;
}

void Camera::SetTarget(Vector3 target){
	_viewStale = true;
	_targ = target;
}

void Camera::SetUp(Vector3 up){
	_viewStale = true;
	_up = up;
}

void Camera::SetFov(OPfloat fov){
	_projStale = true;
	_fov = fov;
}

Matrix4* Camera::GetProj(){
	// if anything has changed, recalculate the projection matrix
	if(_projStale){
		_proj = Matrix4::CreatePerspective(
			_fov,
			_aspect,
			_near,
			_far
		);
		_projStale = false;
	}
	
	return &_proj;
}

Matrix4* Camera::GetView(){
	if(_viewStale){
		_view = Matrix4::CreateLook(
			_pos,
			_targ,
			_up
		);
		_viewStale = false;
	}
	
	return &_view;
}