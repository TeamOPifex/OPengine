
#include "./include/Character3D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"

int CHARACTER_3D_INITIALIZED = 0;
OPmesh CHARACTER_3D_QUAD_MESH;
OPeffect* EFFECT_CHARACTER_3D;

void OPcharacter3DInit(OPeffect* effect) {
	CHARACTER_3D_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_CHARACTER_3D = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_CHARACTER_3D = OPrenderBuildEffect(
			"Common/OPspriteSheet3D.vert",
			"Common/OPspriteSheet.frag",
			OPATTR_POSITION | OPATTR_UV,
			"3D Sprite sheet effect"
			);

		CHARACTER_3D_INITIALIZED = 2;
	}
	else {
		EFFECT_CHARACTER_3D = effect;
		CHARACTER_3D_INITIALIZED = 1;
	}
}

OPcharacter3D* OPcharacter3DCreate(OPsprite** sprites, OPeffect* effect) {
	OPcharacter3D* character = (OPcharacter3D*)OPalloc(sizeof(OPcharacter3D));
	OPbzero(character, sizeof(OPcharacter3D));
	character->Scale = OPvec3One;
	character->Sprites = sprites;
	character->CurrentSprite = sprites[0];
	character->Direction = 1;
	character->FrameRate = 24.0f;
	character->Loop = 1;
	character->Effect = effect;
	if (effect == NULL) {
		character->Effect = EFFECT_CHARACTER_3D;
	}
	return character;
}

void OPcharacter3DDestroy(OPcharacter3D* character) {
	OPfree(character);
}

void OPcharacter3DUpdate(OPcharacter3D* character, OPtimer* timer) {
	character->CurrentSprite->Elapsed += timer->Elapsed;
	if (character->CurrentSprite->Elapsed > 1000 / character->FrameRate) {
		character->CurrentSprite->Elapsed = 0;
		character->CurrentSprite->Frame++;
		if (character->CurrentSprite->Frame >= character->CurrentSprite->FrameCount) {
			if (character->Loop) character->CurrentSprite->Frame = 0;
			else  character->CurrentSprite->Frame--;
		}
	}
}

void OPcharacter3DSetSprite(OPcharacter3D* character, i32 sprite) {
	character->CurrentSprite = character->Sprites[sprite];
	character->CurrentSprite->Frame = 0;
	character->CurrentSprite->Elapsed = 0;
}

OPmat4 world, view, proj;
void OPcharacter3DPrepRender(OPcharacter3D* character, OPcam* camera) {
	OPvec2 frameSize = OPspriteCurrentFrameSize(character->CurrentSprite);
	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	OPrenderBindMesh(&CHARACTER_3D_QUAD_MESH);
	OPrenderBindEffect(character->Effect);

	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);

	OPmat4identity(&world);
	OPmat4rotZ(&world, character->Rotation.z);
	OPvec3 scl = character->Scale / 2.0f;
	scl.x *= character->Direction;
	scl.x *= widthScale;
	scl.y *= heightScale;
	world *= scl;
	world += character->Position;

	OPtextureClearActive();
	OPtextureSmooth();
	OPrenderParami("uColorTexture", OPtextureBind(character->CurrentSprite->Sheet));
	OPrenderParamf("uAlpha", 1.0f);
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
	OPrenderParamVec2("uOffset", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Offset);
	OPrenderParamVec2("uSize", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size);
}

void OPcharacter3DRender(OPcharacter3D* character, OPcam* camera) {
	OPcharacter3DPrepRender(character, camera);
	OPrenderMesh();
}


OPvec2 OPcharacter3DSize(OPcharacter3D* character) {
	OPvec2 frameSize = OPspriteCurrentFrameSize(character->CurrentSprite);

	OPfloat widthScale = frameSize.x / frameSize.y;
	OPfloat heightScale = 1.0f;
	if (widthScale > 1.0f) {
		widthScale = 1.0f;
		heightScale = frameSize.y / frameSize.x;
	}

	f32 width = character->Scale.x * widthScale;
	f32 height = character->Scale.y * heightScale;
	return OPvec2Create(width, height);
}