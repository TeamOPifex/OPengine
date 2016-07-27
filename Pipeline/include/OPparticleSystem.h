#ifndef OPENGINE_PIPELINE_PARTICLESYSTEM
#define OPENGINE_PIPELINE_PARTICLESYSTEM

#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Data/include/OPentHeap.h"
#include "./Math/include/OPvec4.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec2.h"
#include "./Core/include/OPtimer.h"

#ifdef __cplusplus
extern "C"{
#endif

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPparticle {
	OPvec3    Position;     // 12 bytes
	OPvec3    Velocity;     // 12 bytes
	OPfloat   Angle;        // 4 bytes
	OPfloat   AngularVelo;  // 4 bytes
	ui64       Life;         // 4 bytes
	ui64       MaxLife;      // 4 bytes
	OPvec4    Tint;         // 16 bytes
	OPsprite* Animation;    // 4/8 bytes
	ui8       CurrentFrame; // 1 byte
};

struct OPparticleSys {
	OPentHeap* heap;
	OPparticle* particles;
	OPtexture* texture;
	OPeffect* effect;
	OPvec2 uvScale;
	OPfloat fps;
	OPfloat timeElapsed;
};

inline void OPparticleUpdate(OPparticle* p, OPtimer* timer){
	OPvec3 vel = p->Velocity;
	OPfloat dr = p->AngularVelo * timer->Elapsed;
	vel *= (OPfloat)timer->Elapsed;
	p->Position += vel;

	p->Angle += dr;
	p->Life -= timer->Elapsed;
}

inline void OPparticleSysSpawn(OPparticleSys* sys, OPparticle particle){
	OPint ind = -1;
	OPentHeapActivate(sys->heap, &ind);
	if (ind >= 0){
		OPparticle* p = &((OPparticle*)sys->heap->Entities)[ind];
		OPmemcpy(p, &particle, sizeof(OPparticle));
	}
}

void OPparticleSysInit(OPeffect* effect);
OPparticleSys* OPparticleSysCreate(OPtexture* texture, ui16 count, OPeffect* effect);
void OPparticleSysUpdate(OPparticleSys* sys, OPtimer* timer);
void OPparticleSysDestroy(OPparticleSys* sys);
void OPparticleSysDraw(OPparticleSys* sys, OPcam* cam, void(ParticleTransform)(OPparticle*, OPmat4*));

#ifdef __cplusplus
};
#endif


#endif
