// Camera.cpp
#include "./Human/include/Rendering/Camera.h"
#include "./Core/include/Log.h"

OPcam OPcamProj(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect){
	
	OPcam cam = {
		position, target, up,
		fov,
		aspect, near, far,
		{0}, {0},
		0, 0
	};

	OPmat4identity(cam._proj);
	OPmat4identity(cam._view);

	// construct matricies
	OPmat4perspective(cam._proj, fov, aspect, near, far);
	OPmat4look(cam._view, position, target, up);

	return cam;
}

OPcam OPcamOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top){
	OPcam cam = {
		position, target, up,
		0,
		0, near, far,
		{0}, {0},
		0, 0
	};

	OPmat4identity(cam._proj);
	OPmat4identity(cam._view);

	// construct matricies
	OPmat4ortho(cam._proj, left, right, bottom, top, near, far);
	OPmat4look(cam._view, position, target, up);

	return cam;
}