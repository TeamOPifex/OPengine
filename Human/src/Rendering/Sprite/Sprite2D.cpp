#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/Sprite/Sprite2D.h"
#include "./Human/include/Rendering/Primitives/Quad.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Core/include/Assert.h"

int SPRITE_2D_INITIALIZED = 0;
OPmesh SPRITE_2D_QUAD_MESH;
OPsprite2D* CURR_SPRITE_2D = NULL;
OPeffect* EFFECT_SPRITE_2D;

void OPsprite2DInit(OPeffect* effect) {
	SPRITE_2D_QUAD_MESH = OPquadCreate();

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	if (effect == NULL) {
		EFFECT_SPRITE_2D = (OPeffect*)OPalloc(sizeof(OPeffect));

		if (!OPcmanIsLoaded("Common/OPspriteSheet.vert")) OPcmanLoad("Common/OPspriteSheet.vert");
		if (!OPcmanIsLoaded("Common/OPspriteSheet.frag")) OPcmanLoad("Common/OPspriteSheet.frag");

		*EFFECT_SPRITE_2D = OPrenderCreateEffect(
			*(OPshader*)OPcmanGet("Common/OPspriteSheet.vert"),
			*(OPshader*)OPcmanGet("Common/OPspriteSheet.frag"),
			attribs,
			2,
			"Sprite sheet effect"
			);
		SPRITE_2D_INITIALIZED = 2;
	}
	else {
		EFFECT_SPRITE_2D = effect;
		SPRITE_2D_INITIALIZED = 1;
	}
}

void OPsprite2DShutdown() {
	if (SPRITE_2D_INITIALIZED == 2) {
		OPfree(EFFECT_SPRITE_2D);
	}
	SPRITE_2D_INITIALIZED = 0;
}

OPsprite2D* OPsprite2DCreate(OPsprite* sprite) {
	OPsprite2D* val = (OPsprite2D*)OPalloc(sizeof(OPsprite2D));
	OPbzero(val, sizeof(OPsprite2D));
	OPmemcpy(&val->Sprite, sprite, sizeof(OPsprite));
	val->Scale = OPvec2One;
	return val;
}

void OPsprite2DDestroy(OPsprite2D* sprite) {
	OPfree(sprite);
}

void OPsprite2DBind(OPsprite2D* sprite) {
	ASSERT(SPRITE_2D_INITIALIZED, "Sprite2D system has not been initialized");

	CURR_SPRITE_2D = sprite;
}

void OPsprite2DRender() {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");

	OPrenderBindMesh(&SPRITE_2D_QUAD_MESH);
	OPrenderBindEffect(EFFECT_SPRITE_2D);

	OPrenderDepth(0);

	OPmat4 world;
	OPmat4buildRotZ(&world, CURR_SPRITE_2D->Rotation);
	world *= CURR_SPRITE_2D->Scale;
	world *= CURR_SPRITE_2D->Sprite.Frames[CURR_SPRITE_2D->Sprite.Frame].Size;
	world *= OPvec2Create(OPrenderGetAspectRatio(), 1.0);
	world += CURR_SPRITE_2D->Position;
	
	OPtextureClearActive();
	OPtexturePixelate();
	OPrenderParami("uColorTexture", OPtextureBind(CURR_SPRITE_2D->Sprite.Sheet));
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamVec2("uOffset", 1, &CURR_SPRITE_2D->Sprite.Frames[CURR_SPRITE_2D->Sprite.Frame].Offset);
	OPrenderParamVec2("uSize", 1, &CURR_SPRITE_2D->Sprite.Frames[CURR_SPRITE_2D->Sprite.Frame].Size);
	OPrenderMesh();
}


void OPsprite2DMove(OPvec2 xy) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Position += xy;
}
void OPsprite2DMoveX(OPfloat x) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Position.x += x;
}
void OPsprite2DMoveY(OPfloat y) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Position.y += y;
}
void OPsprite2DRotate(OPfloat r) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Rotation += r;
}
void OPsprite2DScale(OPvec2 xy) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Scale *= xy;
}
void OPsprite2DScaleX(OPfloat x) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Scale.x *= x;
}
void OPsprite2DScaleY(OPfloat y) {
	ASSERT(CURR_SPRITE_2D != NULL, "Sprite2D has not been bound");
	CURR_SPRITE_2D->Scale.y *= y;
}