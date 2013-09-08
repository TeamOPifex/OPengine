#pragma once
#ifndef OPEngine_Human_Renderer_Camera
#define OPEngine_Human_Renderer_Camera

#include "./Core/include/MathHelpers.h"
#include "./Math/include/Vector3.h"
#include "./Math/include/Matrix4.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPvec3 _pos, _targ, _up;
	OPfloat _fov;
	OPfloat _aspect, _near, _far;
	OPmat4 _proj, _view;
	OPint _projStale, _viewStale;
} OPcam;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
#define OPcamSetPosition(cam, pos) cam->_pos = pos;cam->_viewStale=1;\
#define OPcamGetPosition(cam) cam->_pos\

#define OPcamSetTarget(cam, targ) cam->_targ = targ;cam->_viewStale=1;\
#define OPcamGetTarget(cam) cam->_targ\

#define OPcamSetUp(cam, up) cam->_up = up;cam->_viewStale=1;\
#define OPcamGetUp(cam) cam->_up\

#define OPcamSetFOV(cam, fov) cam->_fov = fov;cam->_projStale=1;\
#define OPcamGetFOV(cam) cam->_fov\
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPcam OPcamProj (OPvec3 position, OPvec3 target, OPvec3 up, OPfloat near, OPfloat far, OPfloat fov, OPfloat aspect);
OPcam OPcamOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat near, OPfloat far, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif