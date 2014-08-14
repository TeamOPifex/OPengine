#ifndef OP_PERFORMANCE_AABB2
#define OP_PERFORMANCE_AABB2

#include "./Math/include/Matrix4.h"

void OPphysicsInitialize();

void OPphysicsStep();

void OPphysicsDestroy();

void OPphysicsGetTransform(void* shape, OPmat4* mat);
void* OPphysicsBoxCreate();
void* OPphysicsPlaneCreate();

//
//
//#include "./Math/include/Vector2.h"
//#include "./Core/include/MathHelpers.h"
//
//struct OPaabb2 {
//	OPvec2 min;
//	OPvec2 max;
//};
//
//OPint OPaabb2_vs_OPaabb2(OPaabb2* a, OPaabb2* b) {
//	if (a->max.x < b->min.x || a->min.x > b->max.x) return false;
//	if (a->max.y < b->min.y || a->min.y > b->max.y) return false;
//	return true;
//}
//
//struct OPcircle2 {
//	f32 radius;
//	OPvec2 poisition;
//};
//
//OPint OPcircle2_vs_OPcircle2(OPcircle2* a, OPcircle2* b) {
//	f32 r = a->radius + b->radius;
//	r *= r;
//	return r < (a->poisition.x + b->poisition.x) ^ 2 + (a->poisition.y + b->poisition.y) ^ 2;
//}
//
//struct OPphysicsObject2 {
//	OPvec2 velocity;
//	f32 restitution;
//	f32 mass;
//};
//
//void OPresolveCollision(OPphysicsObject2* a, OPphysicsObject2* b) {
//	OPvec2 rv = b->velocity - a->velocity;
//
//	f32 velAlongNormal = OPvec2dot(&rv, normal);
//
//	if (velAlongNormal > 0) return;
//
//	f32 e = __min(a->restitution, b->restitution);
//
//	f32 j = -(1 + e) * velAlongNormal;
//	j /= 1 / a->mass + 1 / b->mass;
//
//	OPvec2 impulse = j * normal;
//	a->velocity -= 1 / a->mass * impulse;
//	b->velocity += 1 / b->mass * impulse;
//}

#endif