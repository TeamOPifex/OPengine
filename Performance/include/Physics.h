#ifndef OP_PERFORMANCE_AABB2
#define OP_PERFORMANCE_AABB2

#include "./Math/include/Matrix4.h"

void OPphysicsInitialize();
void OPphysicsStep();
void OPphysicsDestroy();

void OPphysicsGetTransform(void* shape, OPmat4* mat);
void* OPphysicsBoxCreate(f32 x, f32 y, f32 z);
void* OPphysicsSphereCreate(f32 x, f32 y, f32 z);
void* OPphysicsPlaneCreate();


#endif