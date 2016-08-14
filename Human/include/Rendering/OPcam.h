#pragma once

struct OPcam;
typedef struct OPcam OPcam;

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPray3D.h"

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