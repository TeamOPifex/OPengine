
#include "./Pipeline/include/Sprite3D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"

int SPRITE_3D_INITIALIZED = 0;
OPmesh SPRITE_3D_QUAD_MESH;
OPeffect* EFFECT_SPRITE_3D;

void OPsprite3DInit(OPeffect* effect) {
	SPRITE_3D_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_SPRITE_3D = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_SPRITE_3D = OPrenderGenEffect(
			"Common/OPspriteSheet3D.vert",
			"Common/OPspriteSheet.frag",
			OPATTR_POSITION | OPATTR_UV,
			"3D Sprite sheet effect",
			NULL
			);

		SPRITE_3D_INITIALIZED = 2;
	}
	else {
		EFFECT_SPRITE_3D = effect;
		SPRITE_3D_INITIALIZED = 1;
	}
}

OPsprite3D* OPsprite3DCreate(OPsprite** sprites, OPeffect* effect) {
	OPsprite3D* sprite = (OPsprite3D*)OPalloc(sizeof(OPsprite3D));
	OPbzero(sprite, sizeof(OPsprite3D));
	sprite->Scale = OPvec3One;
	sprite->Sprites = sprites;
	sprite->CurrentSprite = sprites[0];
	sprite->Direction = 1;
	sprite->FrameRate = 24.0f;
	sprite->Loop = 1;
	sprite->Effect = effect;
	if (effect == NULL) {
		sprite->Effect = EFFECT_SPRITE_3D;
	}
	return sprite;
}

void OPsprite3DCreateFill(OPsprite3D* sprite, OPsprite** sprites, OPeffect* effect) {
	OPbzero(sprite, sizeof(OPsprite3D));
	sprite->Scale = OPvec3One;
	sprite->Sprites = sprites;
	sprite->CurrentSprite = sprites[0];
	sprite->Direction = 1;
	sprite->FrameRate = 24.0f;
	sprite->Loop = 1;
	sprite->Effect = effect;
	if (effect == NULL) {
		sprite->Effect = EFFECT_SPRITE_3D;
	}
}

void OPsprite3DDestroy(OPsprite3D* sprite) {
	OPfree(sprite);
}

void OPsprite3DUpdate(OPsprite3D* sprite, ui64 elapsed) {
	sprite->CurrentElapsed += elapsed;
	if (sprite->CurrentElapsed > 1000 / sprite->FrameRate) {
		sprite->CurrentElapsed = 0;
		sprite->CurrentFrame++;
		if (sprite->CurrentFrame >= sprite->CurrentSprite->FrameCount) {
			if (sprite->Loop) {
				sprite->CurrentFrame = 0;
			}
			else  {
				sprite->CurrentFrame--;
			}
		}
	}
}

void OPsprite3DSetSprite(OPsprite3D* sprite, i32 index) {
	sprite->CurrentSprite = sprite->Sprites[index];
	sprite->CurrentFrame = 0;
	sprite->CurrentElapsed = 0;
}

void OPsprite3DPrepReRender(OPsprite3D* sprite, OPvec3 offset, OPfloat rotation) {
	sprite->CurrentSprite->Frame = sprite->CurrentFrame;
	OPvec2 frameSize = OPspriteCurrentFrameSize(sprite->CurrentSprite);
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPmat4 world;
	OPmat4identity(&world);
	world *= OPvec2Create(widthScale, heightScale);
	OPmat4rotZ(&world, rotation + sprite->Rotation.z);
	OPmat4rotY(&world, sprite->Rotation.y);
	OPvec3 scl = sprite->Scale / 2.0f;
	scl.x *= sprite->Direction;
	world *= scl;
	world += offset + sprite->Position;

	OPrenderParamMat4("uWorld", &world);
	OPrenderParamVec2("uOffset", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset);
	OPrenderParamVec2("uSize", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size);
}

void OPsprite3DPrepRender(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation) {
	OPvec2 frameSize = OPspriteCurrentFrameSize(sprite->CurrentSprite);
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPmat4 world, view, proj;
	OPrenderBindMesh(&SPRITE_3D_QUAD_MESH);
	OPrenderBindEffect(sprite->Effect);

	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);

	OPmat4identity(&world);
	world *= OPvec2Create(widthScale, heightScale);
	OPmat4rotZ(&world, rotation + sprite->Rotation.z);
	OPmat4rotY(&world, sprite->Rotation.y);
	OPvec3 scl = sprite->Scale / 2.0f;
	scl.x *= sprite->Direction;
	world *= scl;
	world += offset + sprite->Position;

	OPtextureClearActive();
	//OPtextureSmooth();
	ui32 bind = OPtextureBind(sprite->CurrentSprite->Sheet);

	//OPlog("SpriteSheet %d @ %x", bind, sprite->CurrentSprite->Sheet);
	OPrenderParami("uColorTexture", bind);
	//OPrenderParamf("uAlpha", 1.0f);
	OPrenderParamMat4("uWorld", &world);
	OPrenderParamMat4("uView", &view);
	OPrenderParamMat4("uProj", &proj);
	OPrenderParamVec2("uOffset", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Offset);
	OPrenderParamVec2("uSize", &sprite->CurrentSprite->Frames[sprite->CurrentFrame].Size);
}

void OPsprite3DRender(OPsprite3D* sprite, OPcam* camera) {
	OPsprite3DPrepRender(sprite, camera, OPvec3Zero, 0);
	OPrenderMesh();
}

void OPsprite3DRenderOffsetRot(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation) {
	OPsprite3DPrepRender(sprite, camera, offset, rotation);
	OPrenderMesh();
}


OPvec2 OPsprite3DSize(OPsprite3D* sprite) {
	OPvec2 frameSize = OPspriteCurrentFrameSize(sprite->CurrentSprite);

	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	f32 width = sprite->Scale.x * widthScale;
	f32 height = sprite->Scale.y * heightScale;
	return OPvec2Create(width, height);
}