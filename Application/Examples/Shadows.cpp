#include "./ExampleSelectorState.h"

#include "./OPengine.h"

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
	OPeffect ShadowEffect;
	OPcam ShadowCamera;
	OPvec3 LightPosition;
	OPmat4 ViewShadow;
	OPmat4 ProjShadow;
	OPmat4 BiasShadow;
	OPtexture2D* shadow2D;
	OPcamFreeFlight Camera;
} ShadowsExample;

ShadowsExample shadowsExample;


#define GROUND 5.0f

// Position
// Normal
// UV
OPfloat _quadVertNormData[] = {
  -GROUND,  0, -GROUND,
  0,  1, 0,
  0,  0,

  GROUND,  0, -GROUND,
  0,  1, 0,
  1,  0,

  GROUND, 0, GROUND,
  0,  1, 0,
  1,  1,

  -GROUND, 0, GROUND,
  0,  1, 0,
  0,  1
};

ui16 _quadIndexData[] = {
  0, 1, 2,
  0, 2, 3
};


void ExampleShadowsEnter(OPgameState* last) {

	shadowsExample.LightPosition = OPvec3Create(1, 1, 1);

	// Load the model to be drawn
	shadowsExample.Model.mesh = (OPmesh*)OPcmanLoadGet("basket.opm");
	shadowsExample.Model.world = OPmat4Translate(1, 1, 0);
	shadowsExample.Model.world *= OPmat4Scl(0.1);
	shadowsExample.GroundMesh = OPmeshCreate();
	OPmeshBind(&shadowsExample.GroundMesh);
	OPmeshBuild(
		sizeof(OPfloat) * 8, sizeof(ui16),
		4, 6,
		_quadVertNormData, _quadIndexData
		);
	shadowsExample.Ground.mesh = &shadowsExample.GroundMesh;
	shadowsExample.Ground.world = OPMAT4IDENTITY;
	OPint vertexStride = shadowsExample.Model.mesh->VertexSize;

	shadowsExample.ModelTexture = (OPtexture*)OPcmanLoadGet("noneNorm.png");
	shadowsExample.GroundTexture = (OPtexture*)OPcmanLoadGet("TetrisBlue.png");

	// Create the effect used to draw a shadowed model
	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	OPshader* vert = (OPshader*)OPcmanLoadGet("Common/TexturedShadow.vert");
	OPshader* frag = (OPshader*)OPcmanLoadGet("Common/TexturedShadow.frag");
	shadowsExample.Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		vertexStride
		);

	shadowsExample.BiasShadow = OPmat4Translate(0.5, 0.5, 0.5);
	shadowsExample.BiasShadow *= OPmat4Scl(0.5);

	OPmat4Log("Bias", shadowsExample.BiasShadow);


	// Create the Frame Buffer that the shadow depth will be
	// rendered to.
	ui16 shadowFrameBufferSize = 1024;
	OPtextureDescription desc = {
		shadowFrameBufferSize,
		shadowFrameBufferSize,
		GL_DEPTH_COMPONENT16,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE
		};

	shadowsExample.ShadowFrameBuffer = OPframeBufferCreateShadow(1024, 1024);
	OPframeBufferUnbind();

	shadowsExample.shadow2D = OPtexture2DCreate(&shadowsExample.ShadowFrameBuffer.Texture, NULL, OPvec2Create(0,0), OPvec2Create(1,1));
	shadowsExample.shadow2D->Scale = OPvec2Create(0.25, 0.25);
	shadowsExample.shadow2D->Position = OPvec2Create(0.5, 0.5);

	// Setup the effect that will be used to render the depth
	// onto the frame buffer.
	OPshaderAttribute attribsShadow[] = {
		{ "aPosition", GL_FLOAT, 3 }
	};

	vert = (OPshader*)OPcmanLoadGet("Common/DepthRTT.vert");
	frag = (OPshader*)OPcmanLoadGet("Common/DepthRTT.frag");
	shadowsExample.ShadowEffect = OPeffectCreate(
		*vert,
		*frag,
		attribsShadow,
		1,
		"Shadow Effect",
		vertexStride
		);

	// Create the camera used for the shadow. This is the position
	// and direction of the light being used for shadows.
	OPfloat size = 10;
	shadowsExample.ShadowCamera = OPcamOrtho(
	    OPvec3Create(0, 15, 15),
	    OPvec3Create(0, 0, 0),
	    OPvec3Create(0, 1, 0),
	    0.01f, 50.0f,
	    -size, size,
	    -size, size );



	OPmaterialInit(&shadowsExample.ModelMaterials[0], &shadowsExample.ShadowEffect);
	OPmaterialInit(&shadowsExample.GroundMaterials[0], &shadowsExample.ShadowEffect);

	OPmaterialInit(&shadowsExample.ModelMaterials[1], &shadowsExample.Effect);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uLightDirection", &shadowsExample.ShadowCamera._pos);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uViewShadow", &shadowsExample.ViewShadow);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uProjShadow", &shadowsExample.ProjShadow);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uBias", &shadowsExample.BiasShadow);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uColorTexture", shadowsExample.ModelTexture);
	OPmaterialAddParam(&shadowsExample.ModelMaterials[1], "uShadow", &shadowsExample.ShadowFrameBuffer.Texture);

	OPmaterialInit(&shadowsExample.GroundMaterials[1], &shadowsExample.Effect);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uLightDirection", &shadowsExample.ShadowCamera._pos);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uViewShadow", &shadowsExample.ViewShadow);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uProjShadow", &shadowsExample.ProjShadow);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uBias", &shadowsExample.BiasShadow);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uColorTexture", shadowsExample.GroundTexture);
	OPmaterialAddParam(&shadowsExample.GroundMaterials[1], "uShadow", &shadowsExample.ShadowFrameBuffer.Texture);

	OPcamFreeFlightInit(&shadowsExample.Camera, 3.0f, 3.0f, OPVEC3_ONE, 0.01f, 50.0f);
}

OPint ExampleShadowsUpdate(OPtimer* timer) {


	OPcamFreeFlightUpdate(&shadowsExample.Camera, timer);


	shadowsExample.ShadowCamera._pos.x-= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_J);
	shadowsExample.ShadowCamera._pos.x += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_L);
	shadowsExample.ShadowCamera._pos.z += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_I);
	shadowsExample.ShadowCamera._pos.z -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_K);
	shadowsExample.ShadowCamera._viewStale = 1;


	OPcamGetView(shadowsExample.ShadowCamera, &shadowsExample.ViewShadow);
	OPcamGetProj(shadowsExample.ShadowCamera, &shadowsExample.ProjShadow);

	OPrenderCull(1);
	glCullFace(GL_FRONT);
	OPrenderDepth(1);
	OPframeBufferBind(&shadowsExample.ShadowFrameBuffer);
	OPrenderClear(0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	OPmodelDraw(shadowsExample.Model, &shadowsExample.ModelMaterials[0], shadowsExample.ShadowCamera);

	OPframeBufferUnbind();

	OPrenderCull(0);

	OPrenderClear(1, 1, 1);

	OPmodelDraw(shadowsExample.Model, &shadowsExample.ModelMaterials[1], shadowsExample.Camera.Camera);
	OPmodelDraw(shadowsExample.Ground, &shadowsExample.GroundMaterials[1], shadowsExample.Camera.Camera);
//	OPmodelDraw(shadowsExample.Model, &shadowsExample.ModelMaterials[1], shadowsExample.ShadowCamera);
//	OPmodelDraw(shadowsExample.Ground, &shadowsExample.GroundMaterials[1], shadowsExample.ShadowCamera);

	OPtexture2DRender(shadowsExample.shadow2D);

	OPrenderPresent();


	return false;
}

OPint ExampleShadowsExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_SHADOWS_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SHADOWS = {
	ExampleShadowsEnter,
	ExampleShadowsUpdate,
	ExampleShadowsExit
};