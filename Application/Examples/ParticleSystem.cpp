#include "./Examples/ParticleSystem.h"
#include "./Pipeline/include/ParticleSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

OPgameState GS_EXAMPLE_PARTICLESYSTEM = {
	ExampleParticleSystemEnter,
	ExampleParticleSystemUpdate,
	ExampleParticleSystemExit
};

OPcam* particleSystemCamera;
OPparticleSys* particleSystem;

void ExampleParticleSystemEnter(OPgameState* last) {
	OPcmanLoad("particleSheet.png");

	particleSystemCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*particleSystemCamera = OPcamProj(
		OPvec3One * 10.0,
		OPvec3Zero,
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPparticleSysInit(NULL);
	particleSystem = OPparticleSysCreate((OPtexture*)OPcmanGet("particleSheet.png"), 256, NULL);
}

int ExampleParticleSystemUpdate(OPtimer* time) {
	OPrenderDepth(0);
	OPrenderClear(0, 0, 0);
	
	OPparticle p = {
		{ 0, 0, 0 },
		{ 0, 0.001f, 0 },
		0.01f,
		-0.001f,
		3000.0f,
		3000.0f,
		{ 1.0, 1.0, 1.0f, 1.0f }
	};
	OPparticleSysSpawn(particleSystem, p);
	OPparticleSysUpdate(particleSystem, time);
	OPparticleSysDraw(particleSystem, particleSystemCamera, NULL);

	OPrenderPresent();
	return false;
}

void ExampleParticleSystemExit(OPgameState* next) {
	OPfree(particleSystemCamera);
}