// Camera.cpp
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPrender.h"
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

OPray3D OPcamUnproject(OPcam* cam, i32 screenX, i32 screenY) {

	// [ -1 ... 1 ] range
	OPfloat x = (((OPfloat)screenX / (OPfloat)OPRENDER_SCALED_WIDTH) - 0.5f) * 2.0f;
	OPfloat y = -(((OPfloat)screenY / (OPfloat)OPRENDER_SCALED_HEIGHT) - 0.5f) * 2.0f;

	// Start and End of ray
	OPvec4 rayStart_nds = OPvec4Create(x, y, -1.0, 1.0);
	OPvec4 rayEnd_nds = OPvec4Create(x, y, 0.0f, 1.0);


	// TODO: (garrett) combine into inverseViewProj matrix
	// Which will reduce the below calls as well
	OPmat4 inverseProjectionMatrix;
	OPmat4 inverseViewMatrix;
	OPmat4Inverse(&inverseProjectionMatrix, cam->Proj);
	OPmat4Inverse(&inverseViewMatrix, cam->View);

	OPvec4 rayStartCamera = OPmat4Transform(rayStart_nds, inverseProjectionMatrix);
	rayStartCamera /= rayStartCamera.w;
	OPvec4 rayStartWorld = OPmat4Transform(rayStartCamera, inverseViewMatrix);
	rayStartWorld /= rayStartWorld.w;
	OPvec4 rayEndCamera = OPmat4Transform(rayEnd_nds, inverseProjectionMatrix);
	rayEndCamera /= rayEndCamera.w;
	OPvec4 rayEndWorld = OPmat4Transform(rayEndCamera, inverseViewMatrix);
	rayEndWorld /= rayEndWorld.w;

	OPvec4 rayDirWorld = rayEndWorld - rayStartWorld;

	OPray3D result = {
		// Origin
		OPvec3Create(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z),
		// Direction
		OPvec3Norm(OPvec3Create(rayDirWorld.x, rayDirWorld.y, rayDirWorld.z))
	};

	return result;

}
