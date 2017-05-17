// Camera.cpp
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPlog.h"

#include "./Human/include/Rendering/OPeffect.h"

void OPcam::SetPerspective(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat nearView, OPfloat farView, OPfloat fov, OPfloat aspect){

	//float _fov = (float)((double)fov * 0.01745329251994329576923690768489);
	pos = position;
	this->target = target;
	this->up = up;
	this->fov = OPradians(fov);
	this->aspect = aspect;
	this->nearView = nearView;
	this->farView = farView;

	OPmat4Identity(&proj);
	OPmat4Identity(&view);

	OPvec3 front;
	front.x = OPcos(OPradians(-90.0f) * OPcos(OPradians(0.0f)));
	front.y = OPsin(OPradians(0.0f));
	front.z = OPsin(OPradians(-90.0f)) * OPcos(OPradians(0.0f));

	// construct matricies
	proj = OPmat4Perspective(this->fov, aspect, nearView, farView);
	view = OPmat4LookAt(position, target, up);
	//view = OPmat4LookAt(position, position + front, up);
}

void OPcam::SetOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top){
	this->pos = position;
	this->target = target;
	this->up = up;
	this->fov = 0;
	this->aspect = 0;
	this->nearView = zNear;
	this->farView = zFar;

	OPmat4Identity(&proj);
	OPmat4Identity(&view);

	// construct matricies
	proj = OPmat4Ortho(left, right, bottom, top, zNear, zFar);
	view = OPmat4LookAt(position, target, up);
}

OPray3D OPcam::Unproject(i32 screenX, i32 screenY) {

	// [ -1 ... 1 ] range
	OPfloat x = (((OPfloat)screenX / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width) - 0.5f) * 2.0f;
	OPfloat y = -(((OPfloat)screenY / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height) - 0.5f) * 2.0f;

	// Start and End of ray
	OPvec4 rayStart_nds = OPvec4(x, y, -1.0, 1.0);
	OPvec4 rayEnd_nds = OPvec4(x, y, 0.0f, 1.0);


	// TODO: (garrett) combine into inverseViewProj matrix
	// Which will reduce the below calls as well
	OPmat4 inverseProjectionMatrix;
	OPmat4 inverseViewMatrix;
	OPmat4Inverse(&inverseProjectionMatrix, proj);
	OPmat4Inverse(&inverseViewMatrix, view);

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

void OPcam::UpdateView() {
	view = OPmat4LookAt(
		pos,
		target,
		up);
}

void OPcam::UpdateProj() {
	proj = OPmat4Perspective(
		fov,
		aspect,
		nearView,
		farView);
}

void OPcam::Update() {
	UpdateView();
	//OPcamUpdateProj(cam);
}


void OPcam::Bind() {
	OPeffectSet("uView", &view);
	OPeffectSet("uProj", &proj);
}
