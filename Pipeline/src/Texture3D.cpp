#include "./include/Texture3D.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Primitives/Quad.h"

OPeffect* DEFAULT_TEXTURE3D_EFFECT = NULL;
OPmesh* TEXTURE_3D_QUAD_MESH = NULL;

void LoadDefaultTexture3DEffect() {
	if (DEFAULT_TEXTURE3D_EFFECT != NULL) return;

	DEFAULT_TEXTURE3D_EFFECT = (OPeffect*)OPalloc(sizeof(OPeffect));
	*DEFAULT_TEXTURE3D_EFFECT = OPrenderBuildEffect(
		"Common/Texture3D.vert",
		"Common/Texture.frag",
		OPATTR_POSITION | OPATTR_UV,
		"Texture3D Effect"
		);
}

OPtexture3D* OPtexture3DCreate(OPtexture* texture, OPeffect* effect) {
	OPtexture3D* tex3d = (OPtexture3D*)OPalloc(sizeof(OPtexture3D));

	OPbzero(tex3d, sizeof(OPtexture3D));
	tex3d->Scale = OPvec3One;
	tex3d->Texture = texture;
	tex3d->Effect = effect;

	if (effect == NULL) {
		LoadDefaultTexture3DEffect();
		tex3d->Effect = DEFAULT_TEXTURE3D_EFFECT;
	}

	if (TEXTURE_3D_QUAD_MESH == NULL) {
		TEXTURE_3D_QUAD_MESH = (OPmesh*)OPalloc(sizeof(OPmesh));
		*TEXTURE_3D_QUAD_MESH = OPquadCreate();
	}

	return tex3d;
}

void OPtexture3DDestroy(OPtexture3D* tex3d) {
	OPfree(tex3d);
}

void OPtexture3DPrepRender(OPtexture3D* tex3d, OPcam* camera) {
	OPrenderBindMesh(TEXTURE_3D_QUAD_MESH);
	OPrenderBindEffect(tex3d->Effect);

	OPrenderDepth(0);

	OPmat4 world, view, proj;
	OPcamGetView((*camera), &view);
	OPcamGetProj((*camera), &proj);


	OPmat4identity(&world);
	OPmat4rotZ(&world, tex3d->Rotation.z);
	world *= tex3d->Scale;
	world += tex3d->Position;

	OPtextureClearActive();
	OPtextureSmooth();
	OPrenderParami("uColorTexture", OPtextureBind(tex3d->Texture));
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
}

void OPtexture3DRender(OPtexture3D* tex3d, OPcam* camera) {
	OPtexture3DPrepRender(tex3d, camera);
	OPrenderMesh();
}