#include "./Pipeline/include/OPtexture2D.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

OPeffect* DEFAULT_TEXTURE2D_EFFECT = NULL;
OPmodel* TEXTURE_2D_QUAD_MESH = NULL;

void LoadDefaultTexture2DEffect() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) return;

	DEFAULT_TEXTURE2D_EFFECT = OPNEW(OPeffect("Common/Texture2D.vert", "Common/Texture.frag"));
}

void OPtexture2DUnloadGlobals() {
	if (DEFAULT_TEXTURE2D_EFFECT != NULL) {
		DEFAULT_TEXTURE2D_EFFECT->Destroy();
		OPfree(DEFAULT_TEXTURE2D_EFFECT);
        DEFAULT_TEXTURE2D_EFFECT = NULL;
	}
	if (TEXTURE_2D_QUAD_MESH != NULL) {
		TEXTURE_2D_QUAD_MESH->Destroy();
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
	tex2d->UVScale = OPvec4(uvStart.x, uvStart.y, uvEnd.x, uvEnd.y);

	if (effect == NULL) {
		LoadDefaultTexture2DEffect();
		tex2d->Effect = DEFAULT_TEXTURE2D_EFFECT;
	}
	
	tex2d->Effect->Bind();

	if (TEXTURE_2D_QUAD_MESH == NULL) {
		TEXTURE_2D_QUAD_MESH = OPquadCreate(0.5f, 0.5f, OPVEC2_ZERO, OPVEC2_ONE);
	}

	return tex2d;
}

void OPtexture2DOLD::Destroy() {

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
	OPrenderCull(false);

	// OPmat4 world = OPmat4RotZ(tex2d->Rotation);
	// world = OPmat4Scl(world, tex2d->Scale.x, tex2d->Scale.y, 1.0);
	// world = OPmat4Scl(world, OPrenderGetWidthAspectRatio() * tex2d->Texture->Description.Width / tex2d->Texture->Description.Height, 1.0, 1.0);
	// world += tex2d->Position;

    OPmat4 world = OPMAT4_IDENTITY;
    OPmat4 size = OPMAT4_IDENTITY;
    //size.Translate(tex2d->Position.x / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, tex2d->Position.y / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 0);
    size.Scl(tex2d->Texture->textureDesc.width * tex2d->Scale.x, tex2d->Texture->textureDesc.height * tex2d->Scale.y, 1.0);
	size.Translate(tex2d->Position.x, tex2d->Position.y, 0);
 //   OPmat4 view = OPMAT4_IDENTITY;
	//OPfloat halfWidth = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / 2.0f;
	//OPfloat halfHeight = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / 2.0f;

 //   view = OPmat4Ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);

    //view.Scl(1.0f / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, 1.0f / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 1.0f);

    //world = size * view;


	OPcam cam;
	cam.SetOrtho(OPvec3(0, 0, 1), OPvec3(0), OPvec3(0, 1, 0), 0.1f, 10.0f, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 0);

	world = cam.proj * cam.view * size;

	OPeffectSet("uColorTexture", tex2d->Texture, 0);
	//OPeffectSet("uUVScale", 1, &tex2d->UVScale);
	//OPeffectSet("uWorld", 1, &world);

	OPshaderUniformBuffer* ubo = tex2d->Effect->GetUniformBuffer("testUniformBlock");
	OPshaderUniformBufferUniform* uUVScale = tex2d->Effect->GetUniformBufferUniform(ubo, "uUVScale");
	OPshaderUniformBufferUniform* uWorld = tex2d->Effect->GetUniformBufferUniform(ubo, "uWorld");

	tex2d->Effect->Set(ubo, uWorld, (void*)&world, 0);
	tex2d->Effect->Set(ubo, uUVScale, (void*)&tex2d->UVScale, 0);
	OPRENDERER_ACTIVE->ShaderUniformBuffer.Bind(ubo);
}

void OPtexture2DRender(OPtexture2DOLD* tex2d) {
	OPtexture2DPrepRender(tex2d);
	OPrenderDrawBufferIndexed(0);
}
