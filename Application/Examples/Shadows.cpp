#include "./ExampleSelectorState.h"

#include "./OPengine.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"

#define GROUND 50.0f

// Position
// Normal
// UV
OPfloat _quadVertNormData[] = {
	GROUND,  0, GROUND,
	0,  1, 0,
	0,  0,

	-GROUND,  0, GROUND,
	0,  1, 0,
	1,  0,

	-GROUND, 0, -GROUND,
	0,  1, 0,
	1,  1,

	GROUND, 0, -GROUND,
	0,  1, 0,
	0,  1
};

ui16 _quadIndexData[] = {
	2, 1, 0,
	3, 2, 0
};

class ShadowsExample : public OPgameState {
	OPmodel* Model;
	OPmat4 ModelWorld;
	OPmodel* Ground;
	OPmat4 GroundWorld;
	OPmaterial ModelMaterials[2];
	OPmaterial GroundMaterials[2];
	OPmodel GroundMesh;
	OPtexture* ModelTexture;
	OPtexture* GroundTexture;
	//OPframeBuffer ShadowFrameBuffer;
	OPframeBufferDepth ShadowFrameBuffer;
	OPeffect Effect;
	OPeffect Effect2;
	OPeffect ShadowEffect;
	OPeffect DepthTextureEffect;
	OPcam ShadowCamera;
	OPvec3 LightPosition;
	OPmat4 ViewShadow;
	OPmat4 ProjShadow;
	OPmat4 BiasShadow;
	OPtexture2DOLD* shadow2D;
	OPcamFreeFlight Camera;
	OPtexture2DOLD* DepthTexture;

	i8 ViewFromLight;

	void Init(OPgameState* state) {
		ViewFromLight = 0;

		LightPosition = OPvec3Create(0, 10, 0);

		// Load the model to be drawn
		Model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");
		ModelWorld = OPmat4Translate(0, 0, 0);
		//ModelWorld *= OPmat4Scl(8.0f);

		// Create the ground quad
		OPvertexLayoutBuilder builder;
		builder.Init((ui32)OPattributes::POSITION | (ui32)OPattributes::NORMAL | (ui32)OPattributes::UV);
		OPvertexLayout vertexLayout = builder.Build();
		Ground = OPquadCreateZPlane();
		//GroundMesh.Init(1, vertexLayout);
		//GroundMesh.vertexLayout.stride = sizeof(OPfloat) * 8;
		//GroundMesh.Build(
		//	4, 6, OPindexSize::SHORT,
		//	_quadVertNormData, _quadIndexData
		//);


		//GroundMesh = OPquadCreateZPlane(50, 50);// OPquadCreate(GROUND, GROUND, OPVEC2_ZERO, OPVEC2_ZERO, OPVEC2_ONE);
		//Ground = OPmodel(1, GroundMe->Init((OPmodel**)&GroundMesh, 1);
		GroundWorld = OPmat4Scl(50.0);
		//Ground.world.RotX(OPpi_2)->RotZ(OPpi);
		//Ground.world.RotX(OPpi_2)->RotZ(OPpi);

		// Load up the textures
		ModelTexture = (OPtexture*)OPCMAN.LoadGet("cemetery.png");
		GroundTexture = (OPtexture*)OPCMAN.LoadGet("TetrisBlue.png");


		// Create the effect used to draw a shadowed model
		OPshaderAttribute attribs[] = {
			{ "aPosition", OPshaderElementType::FLOAT, 3 },
			{ "aNormal", OPshaderElementType::FLOAT, 3 },
			{ "aUV", OPshaderElementType::FLOAT, 2 }
		};

		OPshader* vert = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadow.vert");
		OPshader* frag = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadow.frag");

		//OPshader* vert = (OPshader*)OPCMAN.LoadGet("ColoredModel.vert");
		//OPshader* frag = (OPshader*)OPCMAN.LoadGet("ColoredModel.frag");
		Effect.Init(vert, frag);
		Effect2.Init(vert, frag);

		OPmat4Log("Bias", BiasShadow);


		// Create the Frame Buffer that the shadow depth will be
		// rendered to.
		ui16 shadowFrameBufferSize = 1024;
		OPtextureDesc desc = {
			1024,
			1024,
			OPtextureFormat::RGBA,
			OPtextureWrap::CLAMP_TO_BORDER,
			OPtextureFilter::NEAREST
		};


		const ui32 SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		ShadowFrameBuffer.Init(OPtextureDesc(SHADOW_WIDTH, SHADOW_HEIGHT, OPtextureFormat::DEPTH, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT));
		//ShadowFrameBuffer.Init(OPtextureDesc(SHADOW_WIDTH, SHADOW_HEIGHT, OPtextureFormat::DEPTH, OPtextureWrap::REPEAT, OPtextureFilter::NEAREST, OPtextureType::FLOAT));

		DepthTextureEffect.Init("Common/Texture2D.vert", "Common/TextureDepth.frag");
		DepthTexture = OPtexture2DCreate(&ShadowFrameBuffer.texture, &DepthTextureEffect);
		//DepthTexture = OPtexture2DCreate(&ShadowFrameBuffer.texture[0], &DepthTextureEffect);

		vert = (OPshader*)OPCMAN.LoadGet("Common/DepthRTT.vert");
		frag = (OPshader*)OPCMAN.LoadGet("Common/DepthRTT.frag");
		ShadowEffect.Init(vert, frag);

		ModelMaterials[0].Init(&ShadowEffect);
		ModelMaterials[0].cullFace = OPcullFace::FRONT;
		GroundMaterials[0].Init(&ShadowEffect);

		ModelMaterials[1].Init(&Effect);
		ModelMaterials[1].AddParam("uViewShadow", &ShadowCamera.view);
		ModelMaterials[1].AddParam("uProjShadow", &ShadowCamera.proj);
		ModelMaterials[1].AddParam("uAlbedoMap", ModelTexture, 0);
		ModelMaterials[1].AddParam("uShadow", &ShadowFrameBuffer.texture, 1);
		ModelMaterials[1].AddParam("uLightPos", &ShadowCamera.pos);
		ModelMaterials[1].AddParam("uViewPos", &Camera.Camera.pos);

		GroundMaterials[1].Init(&Effect2);
		GroundMaterials[1].AddParam("uViewShadow", &ShadowCamera.view);
		GroundMaterials[1].AddParam("uProjShadow", &ShadowCamera.proj);
		GroundMaterials[1].AddParam("uAlbedoMap", ModelTexture, 0);
		GroundMaterials[1].AddParam("uShadow", &ShadowFrameBuffer.texture, 1);
		GroundMaterials[1].AddParam("uLightPos", &ShadowCamera.pos);
		GroundMaterials[1].AddParam("uViewPos", &Camera.Camera.pos);

		Camera.Init(3.0f, 3.0f, OPvec3(0, 5, 5), 0.01f, 50.0f);
		//ShadowCamera.SetOrtho(OPvec3(-2, 5, -1), OPVEC3_ZERO, OPVEC3_UP, 0.1f, 1000.0f, -10, 10, -10, 10);
		ShadowCamera.SetPerspective(OPvec3(10, 10, 10), OPVEC3_ZERO, OPVEC3_UP, 0.1f, 1000.0f, 45.0f, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height);
	}

	OPint Update(OPtimer* timer) {
		Camera.Update(timer);

		ShadowCamera.pos.x -= 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::J);
		ShadowCamera.pos.x += 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::L);
		ShadowCamera.pos.y -= 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::U);
		ShadowCamera.pos.y += 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::O);
		ShadowCamera.pos.z += 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::I);
		ShadowCamera.pos.z -= 0.01f * timer->Elapsed * OPKEYBOARD.IsDown(OPkeyboardKey::K);
		ShadowCamera.Update();

		if (OPKEYBOARD.WasReleased(OPkeyboardKey::V)) {
			ViewFromLight = !ViewFromLight;
		}

		return false;
	}

	void Render(OPfloat delta) {

		OPrenderCull(1);
		OPrenderDepth(1);


		{
			ShadowFrameBuffer.Bind();

			OPrenderClear(0.1f, 0.1f, 0.1f);

			OPrenderDepth(true);
			OPrenderDepthWrite(true);

			//OPrenderClearDepth();

			Model->Draw(&ModelWorld, &ModelMaterials[0], &ShadowCamera);

			//Ground.Draw(&GroundMaterials[0], &ShadowCamera);

			ShadowFrameBuffer.Unbind();
		}

		OPrenderClear(0.1f, 0.1f, 0.1f);

		if (ViewFromLight) {
			Model->Draw(&ModelWorld, &ModelMaterials[1], &ShadowCamera);
			GroundMesh.Draw(&GroundWorld, &ModelMaterials[1], &ShadowCamera);
		} else {
			Model->Draw(&ModelWorld, &ModelMaterials[1], &Camera.Camera);
			Ground->Draw(&GroundWorld, &ModelMaterials[1], &Camera.Camera);
			//GroundMesh.Draw(&GroundWorld, &GroundMaterials[1], &Camera.Camera);
		}
		//OPtexture2DRender(DepthTexture);

		OPrenderPresent();
	}

	OPint Exit(OPgameState* state) {
		return 0;
	}
};

ShadowsExample _GS_EXAMPLE_SHADOWS;

OPint GS_EXAMPLE_SHADOWS_AVAILABLE = 1;
OPgameState* GS_EXAMPLE_SHADOWS = &_GS_EXAMPLE_SHADOWS;