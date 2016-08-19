#include "./Pipeline/include/OPtexture3D.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

OPeffect* DEFAULT_TEXTURE3D_EFFECT = NULL;
OPmodel* TEXTURE_3D_QUAD_MESH = NULL;

void LoadDefaultTexture3DEffect() {
	if (DEFAULT_TEXTURE3D_EFFECT != NULL) return;

	DEFAULT_TEXTURE3D_EFFECT = (OPeffect*)OPalloc(sizeof(OPeffect));
	DEFAULT_TEXTURE3D_EFFECT->Init("Common/Texture3D.vert", "Common/Texture.frag");
}

OPtexture3D* OPtexture3DCreate(OPtexture* texture, OPeffect* effect) {
	OPtexture3D* tex3d = (OPtexture3D*)OPalloc(sizeof(OPtexture3D));

	OPbzero(tex3d, sizeof(OPtexture3D));
	tex3d->Scale = OPVEC3_ONE;
	tex3d->Texture = texture;
	tex3d->Effect = effect;

	if (effect == NULL) {
		LoadDefaultTexture3DEffect();
		tex3d->Effect = DEFAULT_TEXTURE3D_EFFECT;
	}

	if (TEXTURE_3D_QUAD_MESH == NULL) {
		TEXTURE_3D_QUAD_MESH = OPquadCreate();
	}

	return tex3d;
}

void OPtexture3DDestroy(OPtexture3D* tex3d) {
	OPfree(tex3d);
}

void OPtexture3DPrepRender(OPtexture3D* tex3d, OPcam* camera) {
	TEXTURE_3D_QUAD_MESH->Bind();
	tex3d->Effect->Bind();

	OPrenderDepth(0);

	OPmat4 world;


	OPmat4Identity(&world);
	OPmat4RotZ(&world, tex3d->Rotation.z); 
	world = OPmat4Scl(world, tex3d->Scale.x, tex3d->Scale.y, 1.0);
	world += tex3d->Position;

	OPeffectSet("uColorTexture", tex3d->Texture, 0);
	OPeffectSet("uWorld", 1, &world);
	OPeffectSet("uProj", 1, &camera->proj);
	OPeffectSet("uView", 1, &camera->view);
}

void OPtexture3DRender(OPtexture3D* tex3d, OPcam* camera) {
	OPtexture3DPrepRender(tex3d, camera);
	OPrenderDrawBufferIndexed(0);
}