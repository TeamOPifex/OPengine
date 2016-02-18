#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
	OPtexture* Texture;
} TexturedExample;

TexturedExample* texturedExample;

void ExampleTexturedEnter(OPgameState* last) {
	OPcmanLoad("adobe.opm");
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");
	OPcmanLoad("adobe.png");

	texturedExample = (TexturedExample*)OPalloc(sizeof(TexturedExample));

	texturedExample->Mesh = (OPmesh*)OPcmanGet("adobe.opm");
	texturedExample->Texture = (OPtexture*)OPcmanGet("adobe.png");
	texturedExample->Rotation = 0;

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	texturedExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	*texturedExample->Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		texturedExample->Mesh->vertexLayout.stride
		);

	texturedExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*texturedExample->Camera = OPcamPersp(
		OPVEC3_ONE * 10.0,
		OPvec3Create(0, 0, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);
	OPrenderDepth(1);
}

OPint ExampleTexturedUpdate(OPtimer* time) {
	OPrenderClear(0, 0, 0);

	if (OPkeyboardIsDown(OPKEY_SPACE)) { texturedExample->Rotation++; }

	OPmeshBind(texturedExample->Mesh);
	OPeffectBind(texturedExample->Effect);

	OPmat4 world;
	world = OPmat4RotY(texturedExample->Rotation / 100.0);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(texturedExample->Texture);
	OPeffectParami("uColorTexture", tex);

	OPeffectParamMat4("uWorld", &world);
	OPeffectParamMat4("uProj", &texturedExample->Camera->proj);
	OPeffectParamMat4("uView", &texturedExample->Camera->view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("vLightDirection", &light);

	OPmeshRender();

	OPrenderPresent();
	return false;
}
void ExampleTexturedRender(OPfloat delta) {

}
OPint ExampleTexturedExit(OPgameState* next) {
	OPeffectUnload(texturedExample->Effect);
	OPfree(texturedExample->Effect);
	OPfree(texturedExample->Camera);

	OPfree(texturedExample);

	return 0;
}

OPint GS_EXAMPLE_TEXTURED_AVAILABLE = 1;
OPgameState GS_EXAMPLE_TEXTURED = {
	ExampleTexturedEnter,
	ExampleTexturedUpdate,
	ExampleTexturedRender,
	ExampleTexturedExit
};
