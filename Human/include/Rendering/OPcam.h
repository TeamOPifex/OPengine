#ifndef OPENGINE_HUMAN_RENDERER_CAMERA
#define OPENGINE_HUMAN_RENDERER_CAMERA

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPray3D.h"
#include "./Core/include/OPmath.h"

struct OPcam;
typedef struct OPcam OPcam;

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPcam {
	OPvec3 pos;
	OPvec3 target;
	OPvec3 up;
	OPfloat fov;
	OPfloat aspect;
	OPfloat nearView;
	OPfloat farView;
	OPmat4 proj;
	OPmat4 view;

	void SetPerspective(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat fov, OPfloat aspect);
	void SetOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);
	void Update();
	void UpdateView();
	void UpdateProj(); 
	OPray3D Unproject(i32 x, i32 y);
	void Bind();

	inline void SetPerspective(OPvec3 position, OPvec3 target) {
		SetPerspective(position, target, OPVEC3_UP, 0.1f, 1000.0f, 45.0f, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height);
	}

	inline void SetOrtho(OPvec3 position, OPvec3 target, OPfloat size) {
		SetOrtho(position, target, OPVEC3_UP, 0.1f, 100.0f, size, size, size, size);
	}
};

#endif
