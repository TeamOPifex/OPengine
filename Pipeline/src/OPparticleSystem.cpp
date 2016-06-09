#include "./Pipeline/include/OPparticleSystem.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Math/include/OPmat4.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

int PARTICLE_SYSTEM_INITIALIZED = 0;
OPmesh PARTICLE_SYSTEM_QUAD_MESH;
OPeffect* EFFECT_PARTICLE_SYSTEM;

void OPparticleSysInit(OPeffect* effect) {
	PARTICLE_SYSTEM_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_PARTICLE_SYSTEM = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_PARTICLE_SYSTEM = OPeffectGen(
			"Common/OPparticleSystem.vert",
			"Common/OPparticleSystem.frag",
			OPATTR_POSITION | OPATTR_UV,
			"Particle System effect",
			0
			);
		OPlog("Particle system initialized with effect '%s'", EFFECT_PARTICLE_SYSTEM->Name);
		PARTICLE_SYSTEM_INITIALIZED = 2;
	}
	else {
		EFFECT_PARTICLE_SYSTEM = effect;
		PARTICLE_SYSTEM_INITIALIZED = 1;
	}
}

OPparticleSys* OPparticleSysCreate(OPtexture* texture, ui16 count, OPeffect* effect) {
	OPparticleSys* sys = (OPparticleSys*)OPalloc(sizeof(OPparticleSys));
	sys->texture = texture;
	OPuint entHeapSize = OPentHeapBytes(sizeof(OPparticle), count);
	OPparticle* particles = (OPparticle*)OPalloc(entHeapSize);
	sys->particles = particles;
	sys->heap = OPentHeapCreate(particles, sizeof(OPparticle), count);
	sys->uvScale.x = 0.2f;
	sys->uvScale.y = 0.2f;
	sys->fps = 0;
	sys->timeElapsed = 0;

	ASSERT(effect != NULL || EFFECT_PARTICLE_SYSTEM != NULL, "No effect was provided to the Particle System and a default was not initialized.");
	if (effect != NULL) {
		sys->effect = effect;
	}
	else {
		sys->effect = EFFECT_PARTICLE_SYSTEM;
	}

	return sys;
}

void OPparticleSysUpdate(OPparticleSys* sys, OPtimer* timer) {
	OPint max = 0, i = sys->heap->MaxIndex;
	OPfloat dt = timer->Elapsed / 1000.0f;
	if (i >= 0)
	for (; i--;){
		ASSERT(
			i >= 0,
			"Attempted to update a particle that was out of bounds"
			);
		OPparticle* p = &((OPparticle*)sys->heap->Entities)[i];
		if (p->Life <= 0) continue;

		OPparticleUpdate(p, timer);

		// kill the particle and add it back to the heap
		// if it has lived it's full life
		if (p->Life <= 0){
			OPentHeapKill(sys->heap, i);
		}
	}

	// track the time passed
	sys->timeElapsed += dt;
}

void OPparticleSysDestroy(OPparticleSys* sys) {
	OPfree(sys->particles);
	OPfree(sys);
}

void _OPparticlePrepareFrame(OPparticleSys* sys, OPparticle* p, OPint frameChange){
	ui8 frame = p->CurrentFrame;
	if (p->Animation != NULL) {
		if(frameChange){
			// loop the animation
			frame = p->CurrentFrame = (++p->CurrentFrame) % p->Animation->FrameCount;
		}
		//OPlog("Frame  %d", frame);

	// if this particle system is animated, set the offset uniforms for each particle
	// to indicate the current frame of animation
		OPeffectParamVec2("uTexCoordScale", &p->Animation->Frames[frame].Size);
		OPeffectParamVec2("uSpriteOffset", &p->Animation->Frames[frame].Offset);

		OPtextureClearActive();
		OPeffectParami("uColorTexture", OPtextureBind(p->Animation->Sheet));
	}

}

void OPparticleSysDraw(OPparticleSys* sys, OPcam* cam, void(ParticleTransform)(OPparticle*, OPmat4*)) {
	OPmat4 world;
	OPint frameChange = sys->fps && sys->timeElapsed > (1.0f / sys->fps);

	PARTICLE_SYSTEM_QUAD_MESH.Bind();
	OPeffectBind(EFFECT_PARTICLE_SYSTEM);

	OPeffectParamMat4v("uView", 1, &cam->view);
	OPeffectParamMat4v("uProj", 1, &cam->proj);

	if(!sys->fps){
		OPeffectParamVec2("uTexCoordScale", &sys->uvScale);
		OPeffectParamVec2("uSpriteOffset", (OPvec2*)&OPVEC2_ZERO);
		OPtextureClearActive();
		OPeffectParami("uColorTexture", OPtextureBind(sys->texture));
	}


	if (ParticleTransform != NULL) {
		for (OPint i = sys->heap->MaxIndex; i--;){
			OPparticle* p = &((OPparticle*)sys->heap->Entities)[i];

			if (p->Life <= 0) continue;

			OPmat4Identity(&world);
			OPmat4Scl(&world, 1, 1, 1);
			OPmat4RotZ(&world, p->Angle);
			OPmat4Translate(&world, p->Position.x, p->Position.y, p->Position.z);
			
			if(sys->fps){
				_OPparticlePrepareFrame(sys, p, frameChange);
			}

			OPeffectParamVec4("uTint", (OPvec4*)&p->Tint);
			OPeffectParamMat4v("uWorld", 1, &world);
			OPmeshRender();
		}
	}
	else {

		for (OPint i = sys->heap->MaxIndex; i--;){
			OPparticle* p = &((OPparticle*)sys->heap->Entities)[i];
			if (p->Life <= 0) continue;
			//ParticleTransform(p, &world);

			if(sys->fps){
				_OPparticlePrepareFrame(sys, p, frameChange);
			}

			OPeffectParamVec4("uTint", (OPvec4*)&p->Tint);
			OPeffectParamMat4v("uWorld", 1, &world);
			OPmeshRender();
		}
	}

	if(frameChange){
		sys->timeElapsed = 0;
	}
}