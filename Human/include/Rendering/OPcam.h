#ifndef OPENGINE_HUMAN_RENDERER_CAMERA
#define OPENGINE_HUMAN_RENDERER_CAMERA

#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"
#include "./Core/include/OPmath.h"

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPcam;

struct OPcam{
	OPvec3 _pos;
	OPvec3 _targ;
	OPvec3 _up;
	OPfloat _fov;
	OPfloat _aspect;
	OPfloat _near;
	OPfloat _far;
	OPmat4 Proj;
	OPmat4 View;
	OPint _projStale;
	OPint _viewStale;
	OPcam operator=(OPcam cam) {
		OPmemcpy(this, &cam, sizeof(OPcam));
		return *this;
	}
};

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   

#define OPcamSetPosition(cam, pos) cam->_pos = pos;cam->_viewStale=1;
#define OPcamGetPosition(cam) cam->_pos

#define OPcamSetTarget(cam, targ) cam->_targ = targ;cam->_viewStale=1;
#define OPcamGetTarget(cam) cam->_targ

#define OPcamSetUp(cam, up) cam->_up = up;cam->_viewStale=1;
#define OPcamGetUp(cam) cam->_up

#define OPcamSetFOV(cam, fov) cam->_fov = fov;cam->_projStale=1;
#define OPcamGetFOV(cam) cam->_fov

//-----------------------------------------------------------------------------
#define OPcamUpdateProj(cam){\
	if (cam._projStale){\
	cam.Proj = OPmat4Perspective(\
			cam._fov, \
			cam._aspect, \
			cam._near, \
			cam._far\
			); \
		cam._projStale = false; \
	}\
}\

//-----------------------------------------------------------------------------
#define OPcamUpdateView(cam){\
	if (cam._viewStale){\
		cam.View = OPmat4LookAt(\
		cam._pos, \
		cam._targ, \
		cam._up\
		); \
		cam._viewStale = false; \
	}\
}\

//-----------------------------------------------------------------------------
#define OPcamGetProj(cam, proj){\
	OPcamUpdateProj(cam);\
	(*proj) = cam.Proj;\
}\

//-----------------------------------------------------------------------------
#define OPcamGetView(cam, view){\
	OPcamUpdateView(cam);\
	(*view) = cam.View;\
}\

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
EXPORT_METHOD OPcam OPcamPersp(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat fov, OPfloat aspect);
EXPORT_METHOD OPcam OPcamOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);


#endif