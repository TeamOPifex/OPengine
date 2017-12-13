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

    OPPARTICLESYSTEM_EFFECT =
        OPRENDERER_ACTIVE->Effect.Create(
            (OPshader*)OPCMAN.LoadGet("Common/OPparticleSystem.vert"),
            (OPshader*)OPCMAN.LoadGet("Common/OPparticleSystem.frag"));

    OPPARTICLESYSTEM_SETUP = true;
}

void OPparticleSystem::Shutdown() {
	if (!OPPARTICLESYSTEM_SETUP) return;

	OPPARTICLESYSTEM_QUAD_MESH->Destroy();
	OPfree(OPPARTICLESYSTEM_QUAD_MESH);

	OPPARTICLESYSTEM_EFFECT->Destroy();
	OPfree(OPPARTICLESYSTEM_EFFECT);

	OPPARTICLESYSTEM_SETUP = false;
}

void OPparticleSystem::Init(OPspriteSheet* texture, ui16 count) {
    Startup();
    Init(texture, count, OPPARTICLESYSTEM_EFFECT);
}

void OPparticleSystem::Init(OPspriteSheet* ss, ui16 count, OPeffect* eff) {
    Startup();

	spriteSheet = ss;

	framesPerSecond = 1000.0 / 30.0;

	OPuint entHeapSize = OPentHeap::Bytes(sizeof(OPparticle), count);
	particles = (OPparticle*)OPalloc(entHeapSize);
	heap = OPentHeap::Create(particles, sizeof(OPparticle), count);
	
    if (renderEffect != NULL) {
		renderEffect = eff;
	} else {
		renderEffect = OPPARTICLESYSTEM_EFFECT;
	}
}

OPparticleSystem* OPparticleSystem::Create(OPspriteSheet* ss, ui16 count, OPeffect* eff) {
	OPparticleSystem* sys = (OPparticleSystem*)OPalloc(sizeof(OPparticleSystem));
    sys->Init(ss, count, eff);
    return sys;
}

void OPparticleSystem::UpdateParticle(OPparticle* p, ui64 dt) {
	OPvec3 vel = p->Velocity;
	OPfloat dr = p->AngularVelo * dt;
	vel *= (OPfloat)dt;
	p->Position += vel;

	p->Angle += dr;
	p->Life += dt;
	p->Elapsed += dt;

	p->Scale += (p->ScaleChange * dt);

	if (p->Elapsed > framesPerSecond) {
		p->CurrentFrame = (++p->CurrentFrame) % p->Animation->FrameCount;
		p->Elapsed -= framesPerSecond;
	}
}

void OPparticleSystem::Spawn(OPparticle particle) {
	OPint ind = -1;
	heap->Activate(&ind);
	if (ind >= 0) {
		OPparticle* p = &((OPparticle*)heap->Entities)[ind];
		(*p) = particle;
		//OPmemcpy(p, &particle, sizeof(OPparticle));
	}
}

OPparticle* OPparticleSystem::Spawn(OPsprite* sprite) {
	OPint ind = -1;
	heap->Activate(&ind);
	if (ind >= 0) {
		OPparticle* p = &((OPparticle*)heap->Entities)[ind];
		(*p) = OPparticle(sprite);
		return p;
	}
	return NULL;
}

void OPparticleSystem::Update(ui64 dt) {
	OPint max = 0, i = heap->MaxIndex;

	for (ui32 i = 0; i < heap->MaxIndex; i++) {
		OPparticle* p = &((OPparticle*)heap->Entities)[i];
		if (p->Life > p->MaxLife) continue;

		UpdateParticle(p, dt);


		// kill the particle and add it back to the heap
		// if it has lived it's full life
		if (p->Life > p->MaxLife) {
			heap->Kill(i);
		}
	}
}

void OPparticleSystem::Destroy() {
	heap->Destroy();
	OPfree(particles);
	OPfree(heap);
}

void PrepareFrame(OPparticle* p) {
	ui8 frame = p->CurrentFrame;
	OPeffectSet("uTexCoordScale", &p->Animation->Frames[frame].Size);
	OPeffectSet("uSpriteOffset", &p->Animation->Frames[frame].Offset);
}

void OPparticleSystemRenderParticle(OPparticle* p) {
	OPmat4 world;

	if (p->Life > p->MaxLife) return;

	OPmat4Identity(&world);
	OPmat4Translate(&world, p->Position.x, p->Position.y, p->Position.z);
	OPmat4Scl(&world, p->Scale, p->Scale, p->Scale);
	OPmat4RotZ(&world, p->Angle);

	PrepareFrame(p);

	OPeffectSet("uTint", (OPvec4*)&p->Tint);
	OPeffectSet("uWorld", 1, &world);
	OPrenderDrawBufferIndexed(0);
}

void OPparticleSystem::RenderPrep(OPcam* cam) {
    OPPARTICLESYSTEM_QUAD_MESH->Bind();
    OPPARTICLESYSTEM_EFFECT->Bind();

	OPeffectSet("uView", 1, &cam->view);
	OPeffectSet("uProj", 1, &cam->proj);

    OPeffectSet("uSpriteOffset", (OPvec2*)&OPVEC2_ZERO);
    OPeffectSet("uColorTexture", spriteSheet->Texture, 0);
}

void OPparticleSystem::Render(OPcam* cam) {

	RenderPrep(cam);

	for (OPint i = 0; i < heap->MaxIndex; i++) {
        OPparticle* p = &((OPparticle*)heap->Entities)[i];
		OPparticleSystemRenderParticle(p);
    }
}