#include "./Data/include/ContentManager.h"
#include "./Pipeline/include/Character2D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"
#include "./Human/include/Rendering/Renderer.h"

int CHARACTER_2D_INITIALIZED = 0;
OPmesh CHARACTER_2D_QUAD_MESH;
OPeffect* EFFECT_CHARACTER_2D;

void OPcharacter2DInit(OPeffect* effect) {
	CHARACTER_2D_QUAD_MESH = OPquadCreate();

	if (effect == NULL) {
		EFFECT_CHARACTER_2D = (OPeffect*)OPalloc(sizeof(OPeffect));
		*EFFECT_CHARACTER_2D = OPrenderBuildEffect(
			"Common/OPspriteSheet.vert",
			"Common/OPspriteSheet.frag",
			OPATTR_POSITION | OPATTR_UV,
			"Sprite sheet effect"
			);

		CHARACTER_2D_INITIALIZED = 2;
	}
	else {
		EFFECT_CHARACTER_2D = effect;
		CHARACTER_2D_INITIALIZED = 1;
	}
}

OPcharacter2D* OPcharacter2DCreate(OPsprite** sprites) {
	OPcharacter2D* character = (OPcharacter2D*)OPalloc(sizeof(OPcharacter2D));
	OPbzero(character, sizeof(OPcharacter2D));
	character->Scale = OPvec2One;
	character->Sprites = sprites;
	character->CurrentSprite = sprites[0];
	character->Direction = 1;
	return character;
}

void OPcharacter2DDestroy(OPcharacter2D* character) {
	OPfree(character);
}

void OPcharacter2DUpdate(OPcharacter2D* character, OPtimer* timer) {
	character->CurrentSprite->Elapsed += timer->Elapsed;
	if (character->CurrentSprite->Elapsed > 1000 / 24.0) {
		character->CurrentSprite->Elapsed = 0;
		character->CurrentSprite->Frame++;
		if (character->CurrentSprite->Frame >= character->CurrentSprite->FrameCount) {
			character->CurrentSprite->Frame = 0;
		}
	}
}

void OPcharacter2DSetSprite(OPcharacter2D* character, i32 sprite) {
	character->CurrentSprite = character->Sprites[sprite];
	character->CurrentSprite->Frame = 0;
	character->CurrentSprite->Elapsed = 0;
}

void OPcharacter2DRender(OPcharacter2D* character) {
	OPrenderBindMesh(&CHARACTER_2D_QUAD_MESH);
	OPrenderBindEffect(EFFECT_CHARACTER_2D);

	OPrenderDepth(0);

	OPmat4 world;
	OPmat4buildRotZ(&world, character->Rotation);
	OPvec2 scl = character->Scale;
	scl.x = scl.x * character->Direction;
	world *= scl;
	world *= character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size;
	world *= OPvec2Create(OPrenderGetAspectRatio(), 1.0);
	world += character->Position;

	OPtextureClearActive();
	OPtexturePixelate();
	OPrenderParami("uColorTexture", OPtextureBind(character->CurrentSprite->Sheet));
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamVec2("uOffset", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Offset);
	OPrenderParamVec2("uSize", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size);
	OPrenderMesh();
}









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

OPcharacter3D* OPcharacter3DCreate(OPsprite** sprites) {
	OPcharacter3D* character = (OPcharacter3D*)OPalloc(sizeof(OPcharacter3D));
	OPbzero(character, sizeof(OPcharacter3D));
	character->Scale = OPvec3One;
	character->Sprites = sprites;
	character->CurrentSprite = sprites[0];
	character->Direction = 1;
	return character;
}

void OPcharacter3DDestroy(OPcharacter3D* character) {
	OPfree(character);
}

void OPcharacter3DUpdate(OPcharacter3D* character, OPtimer* timer) {
	character->CurrentSprite->Elapsed += timer->Elapsed;
	if (character->CurrentSprite->Elapsed > 1000 / 24.0) {
		character->CurrentSprite->Elapsed = 0;
		character->CurrentSprite->Frame++;
		if (character->CurrentSprite->Frame >= character->CurrentSprite->FrameCount) {
			character->CurrentSprite->Frame = 0;
		}
	}
}

void OPcharacter3DSetSprite(OPcharacter3D* character, i32 sprite) {
	character->CurrentSprite = character->Sprites[sprite];
	character->CurrentSprite->Frame = 0;
	character->CurrentSprite->Elapsed = 0;
}

OPmat4 world, view, proj;
void OPcharacter3DRender(OPcharacter3D* character, OPcam* camera) {
	OPint w = character->CurrentSprite->Sheet->Description.Width;
	OPint h = character->CurrentSprite->Sheet->Description.Height;
	OPrenderBindMesh(&CHARACTER_3D_QUAD_MESH);
	OPrenderBindEffect(EFFECT_CHARACTER_3D);

	OPrenderDepth(0);

	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);
	OPmat4buildRotZ(&world, character->Rotation.z);
	OPvec3 scl = character->Scale;
	scl.x = scl.x * character->Direction;
	world *= scl;
	scl.x = (character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size.x * w);
	scl.y = (character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size.y * h);
	scl.z = 1.0f;
	world *= scl;
	world += character->Position;
	world += OPvec3Create(0, -(scl.y / 64.0f), 0);

	OPtextureClearActive();
	OPtexturePixelate();
	OPrenderParami("uColorTexture", OPtextureBind(character->CurrentSprite->Sheet));
	OPrenderParamf("uAlpha", 1.0f);
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
	OPrenderParamVec2("uOffset", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Offset);
	OPrenderParamVec2("uSize", 1, &character->CurrentSprite->Frames[character->CurrentSprite->Frame].Size);
	OPrenderMesh();
}