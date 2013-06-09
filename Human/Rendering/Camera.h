#pragma once

#include "./Core/include/MathHelpers.h"
#include "./Human/Math/Vector3.h"
#include "./Human/Math/Matrix4.h"

class Camera{
public:
	Camera(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect);
	Camera(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);

	void SetPosition(Vector3 position);
	Vector3 GetPosition(){ return _pos; }
	void SetTarget(Vector3 target);
	Vector3 GetTarget(){ return _targ; }
	void SetUp(Vector3 up);
	Vector3 GetUp(){ return _up; }
	void SetFov(OPfloat fov);
	OPfloat GetFov(){ return _fov; }

	Matrix4* GetProj();
	Matrix4* GetView();

	static Camera GameCamera;
private:
	Vector3 _pos, _targ, _up;
	OPfloat _fov;
	OPfloat _aspect, _near, _far;
	Matrix4 _proj, _view;
	OPint _projStale, _viewStale;
};