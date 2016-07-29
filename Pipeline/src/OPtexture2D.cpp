#include "./Pipeline/include/OPtexture2D.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

OPeffect* DEFAULT_TEXTURE2D_EFFECT = NULL;
OPmesh* TEXTURE_2D_QUAD_MESH = NULL;

void LoadDefaultTexture2DEffect() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) return;

	DEFAULT_TEXTURE2D_EFFECT = (OPeffect*)OPalloc(sizeof(OPeffect));
	DEFAULT_TEXTURE2D_EFFECT->Init("Common/Texture2D.vert", "Common/Texture.frag");
	DEFAULT_TEXTURE2D_EFFECT->AddUniform("uColorTexture");
	DEFAULT_TEXTURE2D_EFFECT->AddUniform("uWorld");
}

void OPtexture2DUnloadGlobals() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) {
		DEFAULT_TEXTURE2D_EFFECT->Destroy();
		OPfree(DEFAULT_TEXTURE2D_EFFECT);
        DEFAULT_TEXTURE2D_EFFECT = NULL;
	}
	if (TEXTURE_2D_QUAD_MESH != NULL) {
		OPmeshDestroy(TEXTURE_2D_QUAD_MESH);
	//	OPfree(TEXTURE_2D_QUAD_MESH->Vertices);
		OPfree(TEXTURE_2D_QUAD_MESH);
        TEXTURE_2D_QUAD_MESH = NULL;
	}
}

OPtexture2DOLD* OPtexture2DCreate(OPtexture* texture, OPeffect* effect, OPvec2 uvStart, OPvec2 uvEnd) {
	OPtexture2DOLD* tex2d = (OPtexture2DOLD*)OPalloc(sizeof(OPtexture2DOLD));

	OPbzero(tex2d, sizeof(OPtexture2DOLD));
	tex2d->Scale = OPVEC2_ONE;
	tex2d->Texture = texture;
	tex2d->Effect = effect;

	if (effect == NULL) {
		LoadDefaultTexture2DEffect();
		tex2d->Effect = DEFAULT_TEXTURE2D_EFFECT;
	}

	if (TEXTURE_2D_QUAD_MESH == NULL) {
        OPlog("creating quad");
		TEXTURE_2D_QUAD_MESH = (OPmesh*)OPalloc(sizeof(OPmesh));
		*TEXTURE_2D_QUAD_MESH = OPquadCreate(1, 1, uvStart, uvEnd);
	}

	return tex2d;
}

OPtexture2DOLD* OPtexture2DCreate(OPtexture* texture, OPeffect* effect) {
	return OPtexture2DCreate(texture, effect, OPVEC2_ZERO, OPVEC2_ONE);
}

void OPtexture2DDestroy(OPtexture2DOLD* tex2d) {
	OPfree(tex2d);
}

void OPtexture2DPrepRender(OPtexture2DOLD* tex2d) {
	tex2d->Effect->Bind();
	TEXTURE_2D_QUAD_MESH->Bind();

	OPrenderDepth(0);



	// OPmat4 world = OPmat4RotZ(tex2d->Rotation);
	// world = OPmat4Scl(world, tex2d->Scale.x, tex2d->Scale.y, 1.0);
	// world = OPmat4Scl(world, OPrenderGetWidthAspectRatio() * tex2d->Texture->Description.Width / tex2d->Texture->Description.Height, 1.0, 1.0);
	// world += tex2d->Position;

    OPmat4 world = OPMAT4_IDENTITY;
    OPmat4 size = OPMAT4_IDENTITY;
    size.Translate(tex2d->Position.x / (OPfloat)OPRENDER_WIDTH, tex2d->Position.y / (OPfloat)OPRENDER_HEIGHT, 0);
    size.Scl(tex2d->Texture->textureDesc.width * tex2d->Scale.x, tex2d->Texture->textureDesc.height * tex2d->Scale.y, 1.0);
    OPmat4 view = OPMAT4_IDENTITY;
    view.Scl(1.0f / (OPfloat)OPRENDER_WIDTH, 1.0f / (OPfloat)OPRENDER_HEIGHT, 1.0f);

    world = size * view;

	OPeffectSet("uColorTexture", tex2d->Texture);
	OPeffectSet("uWorld", 1, &world);
}

void OPtexture2DRender(OPtexture2DOLD* tex2d) {
	OPtexture2DPrepRender(tex2d);
	OPmeshRender();
}
