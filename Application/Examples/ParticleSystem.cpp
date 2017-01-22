#include "./ExampleSelectorState.h"
#include "./Pipeline/include/OPparticleSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

class ParticleSystemExample : public OPgameState {
	OPcam* Camera;
	OPparticleSystem* ParticleSystem;


	void Init(OPgameState* last) {
		OPCMAN.Load("particleSheet.png");
		OPCMAN.Load("Toys.opss");

		Camera = (OPcam*)OPalloc(sizeof(OPcam));
		Camera->SetPerspective(
			OPvec3(0, 10, 15),
			OPVEC3_ZERO,
			OPvec3Create(0, 1, 0),
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		OPparticleSystem::Startup();
		ParticleSystem = OPparticleSystem::Create(NULL, 1024, NULL);
		ParticleSystem->fps = 30;
	}

	OPint Update(OPtimer* time) {
		OPsprite* sprite = (OPsprite*)OPCMAN.Get("Toys/Dust");
		OPrenderDepth(0);
		OPrenderClear(0, 0, 0);

		OPparticle p = {
			//OPvec3(-5.0f + OPrandom() * 10.0f, 0, -5.0f + OPrandom() * 10.0f),
			OPVEC3_ZERO,
			OPvec3(-0.0025 + OPrandom() * 0.005, 0.005f, -0.0025 + OPrandom() * 0.005),
			0,//0.04f,
			-0.004f,
			3000,
			3000,
			OPvec4(1.0, 1.0, 1.0f, 1.0f),
			sprite,
			0
		};
		//if (OPrandom() > 0.9) {
		ParticleSystem->Spawn(p);
		//}
		ParticleSystem->Update(time);
		ParticleSystem->Draw(Camera);

		return false;
	}
	void Render(OPfloat delta) {
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		OPfree(Camera);
		ParticleSystem->Destroy();
		return 0;
	}
};


OPint GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE = 1;

ParticleSystemExample _GS_EXAMPLE_PARTICLESYSTEM;
OPgameState* GS_EXAMPLE_PARTICLESYSTEM = &_GS_EXAMPLE_PARTICLESYSTEM;