#include "./Data/include/OPcman.h"
#include "./Pipeline/include/OPsprite2D.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Rendering/OPrender.h"

int SPRITE_2D_PIPELINE_INITIALIZED = 0;
OPmesh* SPRITE_2D_QUAD_MESH_PIPELINE;
OPeffect* EFFECT_SPRITE_2D_PIPELINE;

void OPsprite2DInit(OPeffect* effect) {
	SPRITE_2D_QUAD_MESH_PIPELINE = OPquadCreate();

	if (effect == NULL) {
		EFFECT_SPRITE_2D_PIPELINE = (OPeffect*)OPalloc(sizeof(OPeffect));
		EFFECT_SPRITE_2D_PIPELINE->Init("Common/OPspriteSheet.vert", "Common/OPspriteSheet.frag");

		SPRITE_2D_PIPELINE_INITIALIZED = 2;
	}
	else {
		EFFECT_SPRITE_2D_PIPELINE = effect;
		SPRITE_2D_PIPELINE_INITIALIZED = 1;
	}
}

OPsprite2D* OPsprite2DCreate(OPsprite** sprites, OPeffect* effect) {
	OPsprite2D* sprite = (OPsprite2D*)OPallocZero(sizeof(OPsprite2D));
	sprite->Scale = OPVEC2_ONE;
	sprite->Sprites = sprites;
	sprite->CurrentSprite = sprites[0];
	sprite->Direction = 1;
	sprite->FrameRate = 24.0f;
	sprite->Loop = 1;
	sprite->Effect = effect;
	if (effect == NULL) {
		sprite->Effect = EFFECT_SPRITE_2D_PIPELINE;
	}
	return sprite;
}

void OPsprite2DDestroy(OPsprite2D* sprite) {
	OPfree(sprite);
}

void OPsprite2DUpdate(OPsprite2D* sprite, OPtimer* timer) {
	sprite->CurrentElapsed += timer->Elapsed;
	if (sprite->CurrentElapsed > 1000 / sprite->FrameRate) {
		sprite->CurrentElapsed = 0;
		sprite->CurrentFrame++;
		if (sprite->CurrentFrame >= sprite->CurrentSprite->FrameCount) {
			if (sprite->Loop) sprite->CurrentFrame = 0;
			else  sprite->CurrentFrame--;
		}
	}
}

void OPsprite2DSetSprite(OPsprite2D* sprite, i32 index) {
	sprite->CurrentSprite = sprite->Sprites[index];
	sprite->CurrentFrame = 0;
	sprite->CurrentElapsed = 0;
}

void OPsprite2DPrepRender(OPsprite2D* sprite) {
	SPRITE_2D_QUAD_MESH_PIPELINE->Bind();
	sprite->Effect->Bind();


	sprite->CurrentSprite->Frame = sprite->CurrentFrame;
	OPvec2 frameSize = sprite->CurrentSprite->FrameSize();
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPrenderDepth(0);

	OPmat4 world;
	OPmat4Identity(&world);
	OPmat4RotZ(&world, sprite->Rotation);
	OPvec2 scl = sprite->Scale / 2.0;
	scl.x *= sprite->Direction * widthScale * OPrenderGetWidthAspectRatio();
	scl.y *= heightScale;
	world = OPmat4Scl(world, scl.x, scl.y, 1.0);
	world += sprite->Position;

	OPeffectSet("uColorTexture", sprite->CurrentSprite->Sheet, 0);
	OPeffectSet("uWorld", &world);
	OPeffectSet("uOffset", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset);
	OPeffectSet("uSize", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size);
}

void OPsprite2DRender(OPsprite2D* sprite) {
	OPsprite2DPrepRender(sprite);
	OPmeshRender();
}







