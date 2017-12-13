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
	OPfloat   Scale;
	OPfloat   ScaleChange;
	OPvec3    Velocity;     // 12 bytes
	OPfloat   Angle;        // 4 bytes
	OPfloat   AngularVelo;  // 4 bytes
	i64       Life;         // 4 bytes
	i64       MaxLife;      // 4 bytes
	OPvec4    Tint;         // 16 bytes
	OPsprite* Animation;    // 4/8 bytes
	i64		  Elapsed;
	ui8       CurrentFrame; // 1 byte

	OPparticle() { }
	OPparticle(OPsprite* sprite) {
		Position = OPVEC3_ZERO;
		Scale = 1.0;
		ScaleChange = 0.0;
		Velocity = OPVEC3_ZERO;
		Angle = 0;
		AngularVelo = 0;
		Life = 0;
		MaxLife = 1000;
		Tint = OPvec4(1.0);
		Animation = sprite;
		Elapsed = 0;
		CurrentFrame = 0;
	}
};

struct OPparticleSystem {
	OPentHeap* heap;
	OPparticle* particles;

	OPspriteSheet* spriteSheet;
	OPeffect* renderEffect;
	OPfloat framesPerSecond;

    void Init(OPspriteSheet* tex, ui16 maxParticleCount);
    void Init(OPspriteSheet* tex, ui16 maxParticleCount, OPeffect* effect);
	void Update(ui64 dt);
    void Destroy();
	OPparticle* Spawn(OPsprite* sprite);
	void Spawn(OPparticle particle);
	void UpdateParticle(OPparticle* p, ui64 dt);
    void RenderPrep(OPcam* cam);
    void Render(OPcam* cam);


	inline void Update(OPtimer* timer) {
		Update(timer->Elapsed);
	}


    // Setup the Particle System defaults
    static void Startup();
    static void Shutdown();

	static OPparticleSystem* Create(OPspriteSheet* texture, ui16 count, OPeffect* effect);
};
