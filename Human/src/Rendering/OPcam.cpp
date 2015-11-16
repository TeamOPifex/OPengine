// Camera.cpp
#include "./Human/include/Rendering/OPcam.h"
#include "./Core/include/OPlog.h"

OPcam OPcamPersp(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect){

	float _fov = (float)((double)fov * 0.01745329251994329576923690768489);
	OPcam cam = {
		position,
		target,
		up,
		_fov,
		aspect,
		near,
		far,
		{0},
		{0},
		0,
		0
	};

	OPmat4Identity(&cam.Proj);
	OPmat4Identity(&cam.View);

	// construct matricies
	cam.Proj = OPmat4Perspective(_fov, aspect, near, far);
	cam.View = OPmat4LookAt(position, target, up);

	return cam;
}

OPcam OPcamOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top){
	OPcam cam = {
		position, target, up,
		0,
		0, zNear, zFar,
		{0}, {0},
		0, 0
	};

	OPmat4Identity(&cam.Proj);
	OPmat4Identity(&cam.View);

	// construct matricies
	cam.Proj = OPmat4Ortho(left, right, bottom, top, zNear, zFar);
	cam.View = OPmat4LookAt(position, target, up);

	return cam;
}
