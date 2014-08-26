#include "./Examples/ParticleSystem.h"
#include "./Pipeline/include/ParticleSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

OPgameState GS_EXAMPLE_PARTICLESYSTEM = {
	ExampleParticleSystemEnter,
	ExampleParticleSystemUpdate,
	ExampleParticleSystemExit
};

typedef struct {
	OPcam* Camera;
	OPparticleSys* ParticleSystem;
} ParticleSystemExample;

ParticleSystemExample* particleSystemExample;

void ExampleParticleSystemEnter(OPgameState* last) {
	OPcmanLoad("particleSheet.png");

	particleSystemExample = (ParticleSystemExample*)OPalloc(sizeof(ParticleSystemExample));

	particleSystemExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*particleSystemExample->Camera = OPcamProj(
		OPvec3One * 10.0,
		OPvec3Zero,
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPparticleSysInit(NULL);
	particleSystemExample->ParticleSystem = OPparticleSysCreate((OPtexture*)OPcmanGet("particleSheet.png"), 256, NULL);
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
	OPparticleSysSpawn(particleSystemExample->ParticleSystem, p);
	OPparticleSysUpdate(particleSystemExample->ParticleSystem, time);
	OPparticleSysDraw(particleSystemExample->ParticleSystem, particleSystemExample->Camera, NULL);

	OPrenderPresent();
	return false;
}

void ExampleParticleSystemExit(OPgameState* next) {
	OPfree(particleSystemExample->Camera);
	OPparticleSysDestroy(particleSystemExample->ParticleSystem);
	OPfree(particleSystemExample);
}