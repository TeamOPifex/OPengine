#include "./Pipeline/include/OPparticleSystem.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Math/include/OPmat4.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Data/include/OPcman.h"

bool OPPARTICLESYSTEM_SETUP = false;
OPmodel* OPPARTICLESYSTEM_QUAD_MESH;
OPeffect* OPPARTICLESYSTEM_EFFECT;

void OPparticleSystem::Startup() {
    if(OPPARTICLESYSTEM_SETUP) return;

    OPPARTICLESYSTEM_QUAD_MESH = OPquadCreate();

    OPPARTICLESYSTEM_EFFECT = OPNEW(OPeffect);
    OPPARTICLESYSTEM_EFFECT =
        OPRENDERER_ACTIVE->Effect.Create(
            (OPshader*)OPCMAN.LoadGet("Common/OPparticleSystem.vert"),
            (OPshader*)OPCMAN.LoadGet("Common/OPparticleSystem.frag"));

    OPPARTICLESYSTEM_SETUP = true;
}

void OPparticleSystem::Shutdown() {

}

void OPparticleSystem::Init(OPtexture* texture, ui16 count) {
    Startup();
    Init(texture, count, OPPARTICLESYSTEM_EFFECT);
}

void OPparticleSystem::Init(OPtexture* texture, ui16 count, OPeffect* effect) {
    Startup();

    texture = texture;
	OPuint entHeapSize = OPentHeap::Bytes(sizeof(OPparticle), count);
	particles = (OPparticle*)OPalloc(entHeapSize);
	heap = OPentHeap::Create(particles, sizeof(OPparticle), count);
	uvScale.x = 0.2f;
	uvScale.y = 0.2f;
	fps = 0;
	timeElapsed = 0;

    if (effect != NULL) {
		effect = effect;
	} else {
		effect = OPPARTICLESYSTEM_EFFECT;
	}
}

OPparticleSystem* OPparticleSystem::Create(OPtexture* texture, ui16 count, OPeffect* effect) {
	OPparticleSystem* sys = (OPparticleSystem*)OPalloc(sizeof(OPparticleSystem));
    sys->Init(texture, count, effect);
    return sys;
}

void OPparticleSystem::Update(OPtimer* timer) {
	OPint max = 0, i = heap->MaxIndex;
	OPfloat dt = timer->Elapsed / 1000.0f;
	if (i >= 0)
	for (; i--;){
		ASSERT(
			i >= 0,
			"Attempted to update a particle that was out of bounds"
			);
		OPparticle* p = &((OPparticle*)heap->Entities)[i];
		if (p->Life <= 0) continue;
		Update(p, timer);

		// kill the particle and add it back to the heap
		// if it has lived it's full life
		if (p->Life <= 0){
			heap->Kill(i);
		}
	}

	// track the time passed
	timeElapsed += dt;
}

void OPparticleSystem::Destroy() {
	OPfree(particles);
}

void PrepareFrame(OPparticle* p, OPint frameChange) {
	ui8 frame = p->CurrentFrame;
	if (p->Animation != NULL) {
		if(frameChange){
			// loop the animation
			frame = p->CurrentFrame = (++p->CurrentFrame) % p->Animation->FrameCount;
		}
		//OPlog("Frame  %d", frame);

	// if this particle system is animated, set the offset uniforms for each particle
	// to indicate the current frame of animation
		OPeffectSet("uTexCoordScale", &p->Animation->Frames[frame].Size);
		OPeffectSet("uSpriteOffset", &p->Animation->Frames[frame].Offset);

		//OPtextureClearActive();
		OPeffectSet("uColorTexture", p->Animation->Sheet, 0);
	}

}

void OPparticleSystem::_DrawPrep(OPcam* cam) {
    OPPARTICLESYSTEM_QUAD_MESH->Bind();
    OPPARTICLESYSTEM_EFFECT->Bind();

	OPeffectSet("uView", 1, &cam->view);
	OPeffectSet("uProj", 1, &cam->proj);

    if(!fps){
        OPeffectSet("uTexCoordScale", &uvScale);
        OPeffectSet("uSpriteOffset", (OPvec2*)&OPVEC2_ZERO);
        OPeffectSet("uColorTexture", texture, 0);
    }
}

void OPparticleSystem::Draw(OPcam* cam) {
	OPmat4 world;
	OPint frameChange = fps && timeElapsed > (1.0f / fps);
    _DrawPrep(cam);

    for (OPint i = heap->MaxIndex; i--;){
        OPparticle* p = &((OPparticle*)heap->Entities)[i];

        if (p->Life <= 0) continue;

        OPmat4Identity(&world);
        OPmat4Scl(&world, 1, 1, 1);
        OPmat4RotZ(&world, p->Angle);
        OPmat4Translate(&world, p->Position.x, p->Position.y, p->Position.z);

        if(fps){
            PrepareFrame(p, frameChange);
        }

        OPeffectSet("uTint", (OPvec4*)&p->Tint);
        OPeffectSet("uWorld", 1, &world);
        OPrenderDrawBufferIndexed(0);
    }

	if(frameChange){
		timeElapsed = 0;
	}
}

void OPparticleSystem::Draw(OPcam* cam, void(ParticleTransform)(OPparticle*, OPmat4*)) {
	OPmat4 world;
	OPint frameChange = fps && timeElapsed > (1.0f / fps);
    _DrawPrep(cam);

    for (OPint i = heap->MaxIndex; i--;){
        OPparticle* p = &((OPparticle*)heap->Entities)[i];
        if (p->Life <= 0) continue;
        ParticleTransform(p, &world);

        if(fps){
            PrepareFrame(p, frameChange);
        }

        OPeffectSet("uTint", (OPvec4*)&p->Tint);
        OPeffectSet("uWorld", 1, &world);
        OPrenderDrawBufferIndexed(0);
    }

	if(frameChange){
		timeElapsed = 0;
	}
}
