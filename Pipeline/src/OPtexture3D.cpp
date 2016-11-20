#include "./Pipeline/include/OPtexture3D.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

OPeffect* DEFAULT_TEXTURE3D_EFFECT = NULL;
OPmodel* TEXTURE_3D_QUAD_MESH = NULL;

void OPtexture3D_LoadDefaultEffect() {
    // If the default effect has already been loaded then return
	if (DEFAULT_TEXTURE3D_EFFECT != NULL) return;

    // TODO: (garrett) might want to turn this into a smart pointer
    // so that we can auto-kill it when there are no OPtexture3D's
    // that are using it
	DEFAULT_TEXTURE3D_EFFECT = OPNEW(OPeffect);
	DEFAULT_TEXTURE3D_EFFECT->Init("Common/Texture3D.vert", "Common/Texture.frag");
}

OPtexture3D* OPtexture3D::Create(OPtexture* texture, OPeffect* effect) {
	OPtexture3D* tex3d = OPNEW(OPtexture3D);

    tex3d->position = OPVEC3_ZERO;
    tex3d->rotation = OPVEC3_ZERO;
	tex3d->scale = OPVEC3_ONE;
	tex3d->texture = texture;
	tex3d->effect = effect;

    // Ensures the 3D quad to be used has been created
	if (TEXTURE_3D_QUAD_MESH == NULL) {
		TEXTURE_3D_QUAD_MESH = OPquadCreate();
	}

	return tex3d;
}

OPtexture3D* OPtexture3D::Create(OPtexture* texture) {
	OPtexture3D_LoadDefaultEffect();
    return OPtexture3D::Create(texture, DEFAULT_TEXTURE3D_EFFECT);
}

void OPtexture3D::Shutdown() {
    if(DEFAULT_TEXTURE3D_EFFECT != NULL) {
        // destroy the effect
        DEFAULT_TEXTURE3D_EFFECT->Free();
    }

    if (TEXTURE_3D_QUAD_MESH != NULL) {
        // destroy the mesh
		TEXTURE_3D_QUAD_MESH->Free();
	}
}

void OPtexture3D::Destroy() {

}

void OPtexture3D::PrepRender(OPcam* camera) {
	TEXTURE_3D_QUAD_MESH->Bind();

	effect->Bind();

	OPrenderDepth(0);

	OPmat4 world = OPMAT4_IDENTITY;
    world.RotY(rotation.y);
    world.RotZ(rotation.z);
    world.RotX(rotation.x);
    world.Scl(scale.x, scale.y, 1.0);
    world.Translate(position);

	OPeffectSet("uColorTexture", texture, 0);
	OPeffectSet("uWorld", 1, &world);
	OPeffectSet("uProj", 1, &camera->proj);
	OPeffectSet("uView", 1, &camera->view);
}

void OPtexture3D::Render(OPcam* camera) {
	PrepRender(camera);
	OPrenderDrawBufferIndexed(0);
}
