#include "./Pipeline/include/OPtexture2D.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

OPeffect* DEFAULT_TEXTURE2D_EFFECT = NULL;
OPmesh* TEXTURE_2D_QUAD_MESH = NULL;

void LoadDefaultTexture2DEffect() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) return;

	DEFAULT_TEXTURE2D_EFFECT = (OPeffect*)OPalloc(sizeof(OPeffect));
	*DEFAULT_TEXTURE2D_EFFECT = OPeffectGen(
		"Common/Texture2D.vert",
		"Common/Texture.frag",
		OPATTR_POSITION | OPATTR_UV,
		"Texture2D Effect",
		0
		);
}

void OPtexture2DUnloadGlobals() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) {
		OPeffectUnload(DEFAULT_TEXTURE2D_EFFECT);
		OPfree(DEFAULT_TEXTURE2D_EFFECT);
	}
	if (TEXTURE_2D_QUAD_MESH != NULL) {
		OPmeshDestroy(TEXTURE_2D_QUAD_MESH);
		OPfree(TEXTURE_2D_QUAD_MESH->Vertices);
		OPfree(TEXTURE_2D_QUAD_MESH);
	}
}

OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect, OPvec2 uvStart, OPvec2 uvEnd) {
	OPtexture2D* tex2d = (OPtexture2D*)OPalloc(sizeof(OPtexture2D));

	OPbzero(tex2d, sizeof(OPtexture2D));
	tex2d->Scale = OPVEC2_ONE;
	tex2d->Texture = texture;
	tex2d->Effect = effect;

	if (effect == NULL) {
		LoadDefaultTexture2DEffect();
		tex2d->Effect = DEFAULT_TEXTURE2D_EFFECT;
	}

	if (TEXTURE_2D_QUAD_MESH == NULL) {
		TEXTURE_2D_QUAD_MESH = (OPmesh*)OPalloc(sizeof(OPmesh));
		*TEXTURE_2D_QUAD_MESH = OPquadCreate(1, 1, uvStart, uvEnd);
	}

	return tex2d;
}

OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect) {
	return OPtexture2DCreate(texture, effect, OPVEC2_ZERO, OPVEC2_ONE);
}

void OPtexture2DDestroy(OPtexture2D* tex2d) {
	OPfree(tex2d);
}

void OPtexture2DPrepRender(OPtexture2D* tex2d) {
	OPmeshBind(TEXTURE_2D_QUAD_MESH);
	OPeffectBind(tex2d->Effect);

	OPrenderDepth(0);

	OPmat4 world = OPmat4RotZ(tex2d->Rotation);
	world = OPmat4Scl(world, tex2d->Scale.x, tex2d->Scale.y, 1.0);
	world = OPmat4Scl(world, OPrenderGetWidthAspectRatio() * tex2d->Texture->Description.Width / tex2d->Texture->Description.Height, 1.0, 1.0);
	world += tex2d->Position;

	OPtextureClearActive();
	OPeffectParami("uColorTexture", OPtextureBind(tex2d->Texture));
	OPeffectParamMat4v("uWorld", 1, &world);
}

void OPtexture2DRender(OPtexture2D* tex2d) {
	OPtexture2DPrepRender(tex2d);
	OPmeshRender();
}