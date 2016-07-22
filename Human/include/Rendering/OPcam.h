#ifndef OPENGINE_HUMAN_RENDERER_CAMERA
#define OPENGINE_HUMAN_RENDERER_CAMERA

#include "./Human/include/Rendering/OPrender.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPray3D.h"
#include "./Core/include/OPmath.h"

struct OPcam;
typedef struct OPcam OPcam;


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/

#define OPcamSetPosition(cam, pos) (cam)->pos = pos; (cam)->viewStale=1;
#define OPcamGetPosition(cam) (cam)->pos

#define OPcamSetTarget(cam, targ) (cam)->targ = targ; (cam)->viewStale=1;
#define OPcamGetTarget(cam) (cam)->targ

#define OPcamSetUp(cam, up) (cam)->up = up; (cam)->viewStale=1;
#define OPcamGetUp(cam) (cam)->up

#define OPcamSetFOV(cam, fov) (cam)->fov = fov; (cam)->projStale=1;
#define OPcamGetFOV(cam) (cam)->fov


//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPcam OPcamPersp(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat fov, OPfloat aspect);
OPcam OPcamOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top);
OPray3D OPcamUnproject(OPcam* cam, i32 x, i32 y);
void OPcamUpdateView(OPcam* cam);
void OPcamUpdateProj(OPcam* cam);
void OPcamUpdate(OPcam* cam);
void OPcamBind(OPcam* camera);


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

	inline void Update() {
		OPcamUpdate(this);
	}

	inline void SetPerspective(OPvec3 position, OPvec3 target) {
		*this = OPcamPersp(position, target, OPVEC3_UP, 0.1f, 1000.0f, 45.0f, OPWINDOW_ACTIVE->Width / (f32)OPWINDOW_ACTIVE->Height);
	}

	inline void SetPerspective(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat fov, OPfloat aspect) {
		*this = OPcamPersp(position, target, up, zNear, zFar, fov, aspect);
	}

	inline void SetOrtho(OPvec3 position, OPvec3 target, OPfloat size) {
		*this = OPcamOrtho(position, target, OPVEC3_UP, 0.1f, 100.0f, size, size, size, size);
	}

	inline void SetOrtho(OPvec3 position, OPvec3 target, OPvec3 up, OPfloat zNear, OPfloat zFar, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top) {
		*this = OPcamOrtho(position, target, up, zNear, zFar, left, right, bottom, top);
	}
};

#endif
