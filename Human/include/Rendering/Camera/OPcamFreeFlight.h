#pragma once

struct OPcamFreeFlight;
typedef struct OPcamFreeFlight OPcamFreeFlight;

#include "./Human/include/Rendering/OPcam.h"
#include "./Core/include/OPtimer.h"

// Free Flight Camera
struct OPcamFreeFlight {
	OPcam Camera;
	OPvec3 Rotation, Movement;
	OPfloat RotationSpeed, MoveSpeed;

	OPcamFreeFlight() { }
	OPcamFreeFlight(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position) {
		Init(moveSpeed, rotateSpeed, position);
	}
	OPcamFreeFlight(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) { 
		Init(moveSpeed, rotateSpeed, position, camNear, camFar);
	}

	void Init(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar);

	void Update();
	void Update(OPtimer* timer);
	void Destroy();

	inline void Init() {
		Init(5.0f, 1.0f, OPvec3Create(0, 5, 5));
	}

	inline void Init(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position) {
		Init(moveSpeed, rotateSpeed, position, 0.1f, 50.0f);
	}

	inline OPmat4* View() { 
		return &Camera.view; 
	}

	inline OPmat4* Proj() { 
		return &Camera.proj; 
	}

	inline static OPcamFreeFlight* Create() { 
		return OPNEW(OPcamFreeFlight()); 
	}

	inline static OPcamFreeFlight* Create(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position) {
		return OPNEW(OPcamFreeFlight(moveSpeed, rotateSpeed, position));
	}

	inline static OPcamFreeFlight* Create(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) {
		return OPNEW(OPcamFreeFlight(moveSpeed, rotateSpeed, position, camNear, camFar));
	}
};