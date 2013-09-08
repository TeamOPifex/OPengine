// Camera.cpp
#include "./Human/include/Rendering/Camera.h"
#include "./Core/include/Log.h"

OPcam OPcamProj(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect){
	
	OPcam cam = {
		position, target, up,
		fov,
		aspect, near, far,
		Matrix4::SetIdentity(), Matrix4::SetIdentity(),
		0, 0
	};

	// construct matricies
	cam._proj = Matrix4::CreatePerspective(
		fov,
		aspect,
		near,
		far
	);

	cam._view = Matrix4::CreateLook(
		pos,
		targ,
		up
	);

	return cam;
}

OPcam OPcamOrtho(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top){
	OPcam cam = {
		position, target, up,
		fov,
		aspect, near, far,
		Matrix4::SetIdentity(), Matrix4::SetIdentity(),
		0, 0
	};

	// construct matricies
	cam._proj = Matrix4::CreateOrthographic(
		left, right,
		bottom, top,
		_near = near,
		_far = far
	);

	cam._view = Matrix4::CreateLook(
		_pos,
		_targ,
		_up
	);

	return cam;
}