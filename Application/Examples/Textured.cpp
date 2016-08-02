#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect Effect;
	OPcam Camera;
	ui32 Rotation;
	OPtexture* Texture;
} TexturedExample;

TexturedExample* texturedExample;

void ExampleTexturedEnter(OPgameState* last) {
	// OPcmanLoad("adobe.opm");
	// OPcmanLoad("adobe.png");
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");

	texturedExample = (TexturedExample*)OPalloc(sizeof(TexturedExample));

	OPjson* meta = (OPjson*)OPcmanLoadGet("Models/adobe.opm.meta");
	const OPchar* _model = OPjsonString(OPjsonGet(*meta, "model"));
	const OPchar* _texture = OPjsonString(OPjsonGet(*meta, "texture"));

	texturedExample->Mesh = (OPmesh*)OPcmanLoadGet(_model);
	texturedExample->Texture = (OPtexture*)OPcmanLoadGet(_texture);
	texturedExample->Rotation = 0;

	//OPshaderAttribute attribs[] = {
	//	{ "aPosition", GL_FLOAT, 3 },
	//	{ "aNormal", GL_FLOAT, 3 },
	//	{ "aUV", GL_FLOAT, 2 }
	//};

	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	texturedExample->Effect.Init(vert, frag);
	texturedExample->Effect.AddUniform("uColorTexture");
	//texturedExample->Effect->AddUniform("vLightDirection");
	texturedExample->Effect.AddUniform("uWorld");
	texturedExample->Effect.AddUniform("uProj");
	texturedExample->Effect.AddUniform("uView");

	texturedExample->Mesh->vertexLayout.SetOffsets(&texturedExample->Effect);
	texturedExample->Mesh->vertexBuffer.SetLayout(&texturedExample->Mesh->vertexLayout);

	texturedExample->Camera.SetPerspective(
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

	if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { texturedExample->Rotation++; }

	texturedExample->Effect.Bind();
	texturedExample->Mesh->Bind();

	OPmat4 world;
	world = OPmat4RotY(texturedExample->Rotation / 100.0f);

	OPeffectSet("uColorTexture", texturedExample->Texture);

	OPeffectSet("uWorld", &world);
	OPeffectSet("uProj", &texturedExample->Camera.proj);
	OPeffectSet("uView", &texturedExample->Camera.view);

	//OPvec3 light = OPvec3Create(0, 1, 0);
	//OPeffectSet("vLightDirection", &light);

	OPmeshRender();

	OPrenderPresent();
	return false;
}
void ExampleTexturedRender(OPfloat delta) {

}
OPint ExampleTexturedExit(OPgameState* next) {
	texturedExample->Effect.Destroy();

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
