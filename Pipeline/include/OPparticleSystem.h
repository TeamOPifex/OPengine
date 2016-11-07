#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Data/include/OPentHeap.h"
#include "./Math/include/OPvec4.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec2.h"
#include "./Core/include/OPtimer.h"

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

struct OPparticleSystem {
	OPentHeap* heap;
	OPparticle* particles;
	OPtexture* texture;
	OPeffect* effect;
	OPvec2 uvScale;
	OPfloat fps;
	OPfloat timeElapsed;

    void Init(OPtexture* texture, ui16 count);
    void Init(OPtexture* texture, ui16 count, OPeffect* effect);
    void Update(OPtimer* timer);
    void Destroy();
    void _DrawPrep(OPcam* cam);
    void Draw(OPcam* cam);
    void Draw(OPcam* cam, void(ParticleTransform)(OPparticle*, OPmat4*));

    inline void Update(OPparticle* p, OPtimer* timer){
    	OPvec3 vel = p->Velocity;
    	OPfloat dr = p->AngularVelo * timer->Elapsed;
    	vel *= (OPfloat)timer->Elapsed;
    	p->Position += vel;

    	p->Angle += dr;
    	p->Life -= timer->Elapsed;
    }

    inline void Spawn(OPparticle particle){
    	OPint ind = -1;
    	heap->Activate(&ind);
    	if (ind >= 0){
    		OPparticle* p = &((OPparticle*)heap->Entities)[ind];
    		OPmemcpy(p, &particle, sizeof(OPparticle));
    	}
    }

    static OPparticleSystem* Create(OPtexture* texture, ui16 count, OPeffect* effect);

    // Setup the Particle System defaults
    static void Startup();
    static void Shutdown();
};
