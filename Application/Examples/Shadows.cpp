#include "./ExampleSelectorState.h"

#include "./OPengine.h"

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

typedef struct {
	OPmodel Model;
	OPmodel Ground;
	OPmaterial ModelMaterials[2];
	OPmaterial GroundMaterials[2];
	OPmesh GroundMesh;
	OPtexture* ModelTexture;
	OPtexture* GroundTexture;
	OPframeBuffer ShadowFrameBuffer;
	OPeffect Effect;
	OPeffect Effect2;
	OPeffect ShadowEffect;
	OPcam ShadowCamera;
	OPvec3 LightPosition;
	OPmat4 ViewShadow;
	OPmat4 ProjShadow;
	OPmat4 BiasShadow;
	OPtexture2DOLD* shadow2D;
	OPcamFreeFlight Camera;

	i8 ViewFromLight;

	void Init() {
		ViewFromLight = 0;

		LightPosition = OPvec3Create(0, 10, 0);

		// Load the model to be drawn
		Model.mesh = (OPmesh*)OPCMAN.LoadGet("Sheep.opm");
		Model.world = OPmat4Translate(0, 0, 0);
		Model.world *= OPmat4Scl(8.0f);

		// Create the ground quad
		OPvertexLayoutBuilder builder;
		builder.Init((ui32)OPattributes::POSITION | (ui32)OPattributes::NORMAL | (ui32)OPattributes::UV);
		OPvertexLayout vertexLayout = builder.Build();
		GroundMesh = OPmesh(vertexLayout);
		GroundMesh.Build(
			vertexLayout, OPindexSize::SHORT,
			4, 6,
			_quadVertNormData, _quadIndexData
		);
		GroundMesh.vertexLayout.stride = sizeof(OPfloat) * 8;

		//GroundMesh = OPquadCreateZPlane(50, 50);// OPquadCreate(GROUND, GROUND, OPVEC2_ZERO, OPVEC2_ZERO, OPVEC2_ONE);
		Ground.mesh = &GroundMesh;
		Ground.world = OPmat4Scl(1.0);
		//Ground.world.RotX(OPpi_2)->RotZ(OPpi);
		//Ground.world.RotX(OPpi_2)->RotZ(OPpi);

		// Load up the textures
		ModelTexture = (OPtexture*)OPCMAN.LoadGet("noneNorm.png");
		GroundTexture = (OPtexture*)OPCMAN.LoadGet("TetrisBlue.png");


		// Create the effect used to draw a shadowed model
		OPshaderAttribute attribs[] = {
			{ "aPosition", OPshaderElementType::FLOAT, 3 },
			{ "aNormal", OPshaderElementType::FLOAT, 3 },
			{ "aUV", OPshaderElementType::FLOAT, 2 }
		};

		OPshader* vert = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadow.vert");
		OPshader* frag = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadow.frag");
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



		//const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		//GLuint depthMapFBO;
		//glGenFramebuffers(1, &depthMapFBO);

		//OPtextureDesc depthDesc;
		//depthDesc.width = SHADOW_WIDTH;
		//depthDesc.height = SHADOW_HEIGHT;
		//depthDesc.filter = OPtextureFilter::NEAREST;
		//depthDesc.wrap = OPtextureWrap::CLAMP_TO_BORDER;
		//depthDesc.format = OPtextureFormat::NONE;

		//// - Create depth texture
		//GLuint depthMap;
		//glGenTextures(1, &depthMap);
		//glBindTexture(GL_TEXTURE_2D, depthMap);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);



		//ShadowFrameBuffer.Handle = depthMapFBO;
		////ShadowFrameBuffer.Texture.Handle = depthMap;
		//ShadowFrameBuffer.Description.width = SHADOW_WIDTH;
		//ShadowFrameBuffer.Description.height = SHADOW_HEIGHT;
		//ShadowFrameBuffer.Texture.textureDesc.width = SHADOW_WIDTH;
		//ShadowFrameBuffer.Texture.textureDesc.height = SHADOW_HEIGHT;
		////ShadowFrameBuffer = OPframeBufferCreateShadow(1024, 1024);
		////ShadowFrameBuffer = OPframeBufferCreate(desc);

		//shadow2D = OPtexture2DCreate(&ShadowFrameBuffer.Texture);
		//shadow2D->Scale = OPvec2(0.25, 0.25);
		//shadow2D->Position = OPvec2(0.5, 0.5);

		//// Setup the effect that will be used to render the depth
		//// onto the frame buffer.
		//OPshaderAttribute attribsShadow[] = {
		//	{ "aPosition", GL_FLOAT, 3 }
		//};

		//vert = (OPshader*)OPCMAN.LoadGet("Common/DepthRTT.vert");
		//frag = (OPshader*)OPCMAN.LoadGet("Common/DepthRTT.frag");
		//ShadowEffect.Init(vert, frag);

		//// Create the camera used for the shadow. This is the position
		//// and direction of the light being used for shadows.
		//OPfloat size = 30;
		//ShadowCamera.SetOrtho(
		//    OPvec3Create(0, 15, 1),
		//    OPvec3Create(0, 0, 0),
		//    OPvec3Create(0, 1, 0),
		//    0.01f, 120.0f,
		//    -size, size,
		//    -size, size );
		////ShadowCamera = OPcamPersp(
		////	OPvec3Create(0, 15, 0.001f),
		////	OPvec3Create(0, 0, 0),
		////	OPvec3Create(0, 1, 0),
		////	1.0f, 1000.0f,
		////	45.0f, 1.0
		////);


		ModelMaterials[0].Init(&ShadowEffect);
		GroundMaterials[0].Init(&ShadowEffect);

		ModelMaterials[1].Init(&Effect);
		OPmaterialAddParam(&ModelMaterials[1], "uViewShadow", &ShadowCamera.view);
		OPmaterialAddParam(&ModelMaterials[1], "uProjShadow", &ShadowCamera.proj);
		OPmaterialAddParam(&ModelMaterials[1], "uColorTexture", ModelTexture, 0);
		OPmaterialAddParam(&ModelMaterials[1], "uShadow", &ShadowFrameBuffer.Texture, 1);
		OPmaterialAddParam(&ModelMaterials[1], "uLightPos", &ShadowCamera.pos);
		OPmaterialAddParam(&ModelMaterials[1], "uViewPos", &Camera.Camera.pos);

		OPmaterialInit(&GroundMaterials[1], &Effect2);
		OPmaterialAddParam(&GroundMaterials[1], "uViewShadow", &ShadowCamera.view);
		OPmaterialAddParam(&GroundMaterials[1], "uProjShadow", &ShadowCamera.proj);
		OPmaterialAddParam(&GroundMaterials[1], "uColorTexture", ModelTexture, 0);
		OPmaterialAddParam(&GroundMaterials[1], "uShadow", &ShadowFrameBuffer.Texture, 1);
		OPmaterialAddParam(&GroundMaterials[1], "uLightPos", &ShadowCamera.pos);
		OPmaterialAddParam(&GroundMaterials[1], "uViewPos", &Camera.Camera.pos);

		Camera.Init(3.0f, 3.0f, OPVEC3_ONE, 0.01f, 50.0f);
	}

	OPint Update(OPtimer* timer) {
		Camera.Update(timer);

		ShadowCamera.pos.x -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::J);
		ShadowCamera.pos.x += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::L);
		ShadowCamera.pos.y -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::U);
		ShadowCamera.pos.y += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::O);
		ShadowCamera.pos.z += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::I);
		ShadowCamera.pos.z -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPkeyboardKey::K);
		ShadowCamera.Update();
		
		if (OPkeyboardWasReleased(OPkeyboardKey::V)) {
			ViewFromLight = !ViewFromLight;
		}

		return false;
	}

	void Render(OPfloat delta) {
		
		OPrenderCull(1);
		OPrenderDepth(1);

		{
			ShadowFrameBuffer.Bind();

			OPrenderClear(0, 0, 0);

			Model.Draw(&ModelMaterials[0], &ShadowCamera);

			//Ground.Draw(&GroundMaterials[0], &ShadowCamera);

			ShadowFrameBuffer.Unbind();
		}
		
		OPrenderClear(0.1f, 0.1f, 0.1f);

		if (ViewFromLight) {
			Model.Draw(&ModelMaterials[1], &ShadowCamera);
			Ground.Draw(&ModelMaterials[1], &ShadowCamera);
		} else {
			Model.Draw(&ModelMaterials[1], &Camera.Camera);
			Ground.Draw(&GroundMaterials[1], &Camera.Camera);
		}

		//OPtexture2DRender(shadow2D);

		OPrenderPresent();
	}

	void Exit() {

	}
} ShadowsExample;

ShadowsExample shadowsExample;


void ExampleShadowsEnter(OPgameState* last) {
	shadowsExample.Init();
}

OPint ExampleShadowsUpdate(OPtimer* timer) {
	return shadowsExample.Update(timer);
}

void ExampleShadowsRender(OPfloat delta) {
	shadowsExample.Render(delta);
}

OPint ExampleShadowsExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_SHADOWS_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SHADOWS = {
	ExampleShadowsEnter,
	ExampleShadowsUpdate,
	ExampleShadowsRender,
	ExampleShadowsExit
};
