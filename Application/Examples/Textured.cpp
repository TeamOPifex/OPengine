#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"

class TexturedExample : public OPgameState {
	OPmodel* Mesh;
	OPeffect Effect;
	OPcam Camera;
	i32 Rotation;
	OPtexture* Texture;

	void Init(OPgameState* last) {
		// OPCMAN.Load("adobe.opm");
		// OPCMAN.Load("adobe.png");
		OPCMAN.Load("Common/Texture.frag");
		OPCMAN.Load("Common/Texture3D.vert");

		//OPjson* meta = (OPjson*)OPCMAN.LoadGet("Models/adobe.opm.meta");
		const OPchar* _model = "swordsman.opm";// meta->Get("model").String();
		const OPchar* _texture = "swordsman.png";// meta->Get("texture").String();


		Mesh = (OPmodel*)OPCMAN.LoadGet(_model);
		Texture = (OPtexture*)OPCMAN.LoadGet(_texture);
		Rotation = 0;

		//OPshaderAttribute attribs[] = {
		//	{ "aPosition", GL_FLOAT, 3 },
		//	{ "aNormal", GL_FLOAT, 3 },
		//	{ "aUV", GL_FLOAT, 2 }
		//};

		OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
		OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
		Effect.Init(vert, frag);
		Effect.AddUniform("uColorTexture");
		//Effect->AddUniform("vLightDirection");
		Effect.AddUniform("uWorld");
		Effect.AddUniform("uProj");
		Effect.AddUniform("uView");

		Mesh->vertexLayout.SetOffsets(&Effect);
		Mesh->vertexArray.SetLayout(&Mesh->vertexLayout);

		Camera.SetPerspective(
			OPVEC3_ONE * 50.0,
			OPvec3Create(0, 0, 0),
			OPvec3Create(0, 1, 0),
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);
		OPrenderDepth(1);
	}

	OPint Update(OPtimer* time) {
		OPrenderClear(0, 0, 0);

		//if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { 
			//Rotation+=5; 
		//}

		f32 xAmnt = OPTOUCH.PositionMovedX();

		Rotation -= xAmnt;

		Effect.Bind();
		Mesh->Bind();

		OPmat4 world;
		world = OPmat4RotY(Rotation / 100.0f);
		world.Scl(0.1f);

		OPeffectSet("uColorTexture", Texture, 0);

		OPeffectSet("uWorld", &world);
		OPeffectSet("uProj", &Camera.proj);
		OPeffectSet("uView", &Camera.view);

		//OPvec3 light = OPvec3Create(0, 1, 0);
		//OPeffectSet("vLightDirection", &light);

		OPrenderCullMode(OPcullFace::BACK);
		OPrenderDepth(true);
		OPrenderDepthWrite(true);
		OPrenderDrawBufferIndexed(0);

		OPrenderPresent();
		return false;
	}
	void Render(OPfloat delta) {

	}
	OPint Exit(OPgameState* next) {
		Effect.Destroy();

		return 0;
	}
};



OPint GS_EXAMPLE_TEXTURED_AVAILABLE = 1;
TexturedExample _GS_EXAMPLE_TEXTURED;
OPgameState* GS_EXAMPLE_TEXTURED = &_GS_EXAMPLE_TEXTURED;
