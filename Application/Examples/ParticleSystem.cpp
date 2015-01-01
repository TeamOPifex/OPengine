#include "./ExampleSelectorState.h"
#include "./Pipeline/include/OPparticleSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

void ExampleParticleSystemEnter(OPgameState* last);
OPint ExampleParticleSystemUpdate(OPtimer* time);
void ExampleParticleSystemExit(OPgameState* next);

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
	OPcmanLoad("Toys.opss");

	particleSystemExample = (ParticleSystemExample*)OPalloc(sizeof(ParticleSystemExample));

	particleSystemExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*particleSystemExample->Camera = OPcamPersp(
		OPVEC3_ONE * 10.0,
		OPVEC3_ZERO,
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	OPparticleSysInit(NULL);
	particleSystemExample->ParticleSystem = OPparticleSysCreate(NULL, 256, NULL);
	particleSystemExample->ParticleSystem->fps = 10;
}

OPint ExampleParticleSystemUpdate(OPtimer* time) {
	OPsprite* sprite = (OPsprite*)OPcmanGet("Toys/Dust");
	OPrenderDepth(0);
	OPrenderClear(0, 0, 0);
	
	OPparticle p = {
		{ 0, 0, 0 },
		{ 0, 0.001f, 0 },
		0.01f,
		-0.001f,
		3000.0f,
		3000.0f,
		{ 1.0, 1.0, 1.0f, 1.0f },
		sprite
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