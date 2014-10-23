#include "./Data/include/ContentManager.h"
#include "./Pipeline/include/Sprite2D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"
#include "./Human/include/Rendering/Renderer.h"

int SPRITE_2D_PIPELINE_INITIALIZED = 0;
OPmesh SPRITE_2D_QUAD_MESH_PIPELINE;
OPeffect* EFFECT_SPRITE_2D_PIPELINE;

void OPsprite2DInit(OPeffect* effect) {
	SPRITE_2D_QUAD_MESH_PIPELINE = OPquadCreate();

	if (effect == NULL) {
		EFFECT_SPRITE_2D_PIPELINE = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_SPRITE_2D_PIPELINE = OPrenderGenEffect(
			"Common/OPspriteSheet.vert",
			"Common/OPspriteSheet.frag",
			OPATTR_POSITION | OPATTR_UV,
			"Sprite sheet effect",
			0
			);

		SPRITE_2D_PIPELINE_INITIALIZED = 2;
	}
	else {
		EFFECT_SPRITE_2D_PIPELINE = effect;
		SPRITE_2D_PIPELINE_INITIALIZED = 1;
	}
}

OPsprite2D* OPsprite2DCreate(OPsprite** sprites, OPeffect* effect) {
	OPsprite2D* sprite = (OPsprite2D*)OPallocZero(sizeof(OPsprite2D));
	sprite->Scale = OPvec2One;
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
	OPrenderBindMesh(&SPRITE_2D_QUAD_MESH_PIPELINE);
	OPrenderBindEffect(sprite->Effect);


	sprite->CurrentSprite->Frame = sprite->CurrentFrame;
	OPvec2 frameSize = OPspriteCurrentFrameSize(sprite->CurrentSprite);
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPrenderDepth(0);

	OPmat4 world;
	OPmat4identity(&world);
	OPmat4rotZ(&world, sprite->Rotation);
	OPvec2 scl = sprite->Scale / 2.0;
	scl.x *= sprite->Direction * widthScale * OPrenderGetWidthAspectRatio();
	scl.y *= heightScale;
	world *= scl;
	world += sprite->Position;

	OPtextureClearActive();
	OPtextureSmooth();
	OPrenderParami("uColorTexture", OPtextureBind(sprite->CurrentSprite->Sheet));
	OPrenderParamMat4("uWorld", &world);
	OPrenderParamVec2("uOffset", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset);
	OPrenderParamVec2("uSize", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size);
}

void OPsprite2DRender(OPsprite2D* sprite) {
	OPsprite2DPrepRender(sprite);
	OPrenderMesh();
}







