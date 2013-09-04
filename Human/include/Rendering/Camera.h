#pragma once

#include "./Core/include/MathHelpers.h"
#include "./Human/include/Math/Vector3.h"
#include "./Human/include/Math/Matrix4.h"

typedef struct{
	Vector3 _pos, _targ, _up;
	OPfloat _fov;
	OPfloat _aspect, _near, _far;
	Matrix4 _proj, _view;
	OPint _projStale, _viewStale;
} OPcam;

OPcamProj (Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect);
OPcamOrtho(Vector3 position, Vector3 target, Vector3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);

#define OPcamSetPosition(cam, pos) cam->_pos = pos;cam->_viewStale=1;\
#define OPcamGetPosition(cam) cam->_pos\

#define OPcamSetTarget(cam, targ) cam->_targ = targ;cam->_viewStale=1;\
#define OPcamGetTarget(cam) cam->_targ\

#define OPcamSetUp(cam, up) cam->_up = up;cam->_viewStale=1;\
#define OPcamGetUp(cam) cam->_up\

#define OPcamSetFOV(cam, fov) cam->_fov = fov;cam->_projStale=1;\
#define OPcamGetFOV(cam) cam->_fov\

#define OPcamGetProj(cam, proj){\
	if(cam->_projStale){\
		cam->_proj = Matrix4::CreatePerspective(\
			cam->_fov,\
			cam->_aspect,\
			cam->_near,\
			cam->_far\
		);\
		cam->_projStale = false;\
	}\
	proj = &cam->_proj;\
}\

#define OPcamGetView(cam, view){\
	if(cam->_viewStale){\
		cam->_view = Matrix4::CreateLook(\
			cam->_pos,\
			cam->_targ,\
			cam->_up\
		);\
		cam->_viewStale = false;\
	}\
	view = &cam->_view;\
}\
