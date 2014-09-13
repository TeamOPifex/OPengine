#include "./Pipeline/include/ParticleSystem.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Math/include/Matrix4.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/Primitives/Quad.h"

int PARTICLE_SYSTEM_INITIALIZED = 0;
OPmesh PARTICLE_SYSTEM_QUAD_MESH;
OPeffect* EFFECT_PARTICLE_SYSTEM;

void OPparticleSysInit(OPeffect* effect) {
	PARTICLE_SYSTEM_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_PARTICLE_SYSTEM = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_PARTICLE_SYSTEM = OPrenderGenEffect(
			"Common/OPparticleSystem.vert",
			"Common/OPparticleSystem.frag",
			OPATTR_POSITION | OPATTR_UV,
			"Particle System effect",
			NULL
			);

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
}

void OPparticleSysDestroy(OPparticleSys* sys) {
	OPfree(sys->particles);
	OPfree(sys);
}

void OPparticleSysDraw(OPparticleSys* sys, OPcam* cam, void(ParticleTransform)(OPparticle*, OPmat4*)) {
	OPmat4 world;

	OPrenderBindMesh(&PARTICLE_SYSTEM_QUAD_MESH);
	OPrenderBindEffect(EFFECT_PARTICLE_SYSTEM);

	OPrenderParamMat4v("uView", 1, &cam->View);
	OPrenderParamMat4v("uProj", 1, &cam->Proj);
	OPrenderParamVec2("uTexCoordScale", &sys->uvScale);
	OPrenderParamVec2("uSpriteOffset", (OPvec2*)&OPvec2Zero);
	OPrenderParamVec4("uTint", (OPvec4*)&OPvec4One);

	OPtextureClearActive();
	OPrenderParami("uColorTexture", OPtextureBind(sys->texture));

	if (ParticleTransform == NULL) {
		for (OPint i = sys->heap->MaxIndex; i--;){
			OPparticle* p = &((OPparticle*)sys->heap->Entities)[i];
			if (p->Life <= 0) continue;
			
			OPmat4identity(&world);
			OPmat4scl(&world, 1, 1, 1);
			OPmat4rotZ(&world, p->Angle);
			OPmat4translate(&world, p->Position.x, p->Position.y, p->Position.z);

			OPrenderParamMat4v("uWorld", 1, &world);
			OPrenderMesh();
		}
	}
	else {

		for (OPint i = sys->heap->MaxIndex; i--;){
			OPparticle* p = &((OPparticle*)sys->heap->Entities)[i];
			if (p->Life <= 0) continue;
			ParticleTransform(p, &world);
			OPrenderParamMat4v("uWorld", 1, &world);
			OPrenderMesh();
		}
	}
}