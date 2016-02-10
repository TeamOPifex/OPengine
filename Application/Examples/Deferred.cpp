#include "./ExampleSelectorState.h"
#include "./OPengine.h"
#include "./Human/include/Utilities/Geometry.h"


typedef struct {
	OPframeBuffer FrameBuffer;
	OPtexture Position;
	OPtexture Diffuse;
	OPtexture Normal;
	OPtexture Depth;
	OPtexture2D* Position2D;
	OPtexture2D* Diffuse2D;
	OPtexture2D* Normal2D;
	OPtexture2D* Depth2D;
} DeferredGBuffer;

DeferredGBuffer* DeferredGBufferCreate() {
	DeferredGBuffer* gbuffer = (DeferredGBuffer*)OPalloc(sizeof(DeferredGBuffer));
//
//	OPtextureDescription desc = {
//		OPrenderWidth, OPrenderHeight,
//		GL_RGB32F, GL_RGB, GL_FLOAT,
//		OPtextureLinear, OPtextureLinear,
//		OPtextureClamp, OPtextureClamp
//	};
//	gbuffer->FrameBuffer = OPframeBufferCreate(desc);
//	OPframeBufferBind(&gbuffer->FrameBuffer);
//
//	gbuffer->Position = OPtextureCreate(desc);
//	OPframeBufferAttach(&gbuffer->Position, 0);
//	gbuffer->Diffuse = OPtextureCreate(desc);
//	OPframeBufferAttach(&gbuffer->Diffuse, 1);
//	gbuffer->Normal = OPtextureCreate(desc);
//	OPframeBufferAttach(&gbuffer->Normal, 2);
//	desc.InternalFormat = GL_DEPTH_COMPONENT32F;
//	desc.Format = GL_DEPTH_COMPONENT;
//	gbuffer->Depth = OPtextureCreate(desc);
//	OPframeBufferAttachDepth(&gbuffer->Depth);
//
//	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
//		GL_COLOR_ATTACHMENT1,
//		GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, DrawBuffers);
//
//
//	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//	if (Status != GL_FRAMEBUFFER_COMPLETE) {
//		OPlog("FB error, status: 0x%x\n", Status);
//		return false;
//	}
//
//	OPframeBufferUnbind();
//
//
//	gbuffer->Position2D = OPtexture2DCreate(&gbuffer->Position, NULL);
//	gbuffer->Position2D->Scale = OPvec2One * 0.25;
//	gbuffer->Position2D->Position = OPvec2Create(-0.75, 0.75);
//
//	gbuffer->Diffuse2D = OPtexture2DCreate(&gbuffer->Diffuse, NULL);
//	gbuffer->Diffuse2D->Scale = OPvec2One * 0.25;
//	gbuffer->Diffuse2D->Position = OPvec2Create(0.75, 0.75);
//
//	gbuffer->Normal2D = OPtexture2DCreate(&gbuffer->Normal, NULL);
//	gbuffer->Normal2D->Scale = OPvec2One * 0.25;
//	gbuffer->Normal2D->Position = OPvec2Create(0.75, -0.75);
//
//	gbuffer->Depth2D = OPtexture2DCreate(&gbuffer->Depth, NULL);
//	gbuffer->Depth2D->Scale = OPvec2One * 0.25;
//	gbuffer->Depth2D->Position = OPvec2Create(-0.75, -0.75);
//
	return gbuffer;
}

typedef struct {
	OPmesh* Mesh;
	OPeffect Effect;
	OPeffect DeferredEffect;
	OPcam Camera;
	ui32 Rotation;
	DeferredGBuffer* GBuffer;
	OPvec3 LightDirection;
} DeferredExample;

DeferredExample* deferredExample;

void ExampleDeferredEnter(OPgameState* last) {
//	OPcmanLoad("PuzzleBlock.opm");
//	OPcmanLoad("SimpleModel.frag");
//	OPcmanLoad("SimpleModel.vert");
//
//	deferredExample = (DeferredExample*)OPallocZero(sizeof(DeferredExample));
//	deferredExample->GBuffer = DeferredGBufferCreate();
//
//	deferredExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
//
//	deferredExample->Effect = OPeffectGen(
//		"SimpleModel.vert",
//		"SimpleModel.frag",
//		OPATTR_POSITION | OPATTR_NORMAL,
//		"Model Effect",
//		deferredExample->Mesh->VertexSize);
//
//	deferredExample->DeferredEffect = OPeffectGen(
//		"Common/deferred.vert",
//		"Common/deferred.frag",
//		OPATTR_POSITION | OPATTR_NORMAL,
//		"Deferred Effect",
//		deferredExample->Mesh->VertexSize);
//
//	deferredExample->Camera = OPcamPersp(
//		OPvec3One * 2.0,
//		OPvec3Create(0, 1, 0),
//		OPvec3Create(0, 1, 0),
//		0.1f,
//		1000.0f,
//		45.0f,
//		OPrenderWidth / (f32)OPrenderHeight
//		);
//
//	deferredExample->LightDirection = OPvec3Create(0, 1, 0);
//
//	deferredExample->Mesh = OPgeoCreateSphere(1, 10, 10);
}

OPint ExampleDeferredUpdate(OPtimer* time) {

	// Update
	//if (OPkeyboardIsDown(OPKEY_SPACE)) deferredExample->Rotation++;
	//OPmat4 world = OPmat4createRotY(deferredExample->Rotation / 100.0);

	//// Render
	//OPframeBufferBind(&deferredExample->GBuffer->FrameBuffer);
	//OPrenderClearAlpha(0, 0, 0, 0);
	//OPrenderDepth(1);

	//OPbindMeshEffectWorldCam(deferredExample->Mesh, &deferredExample->DeferredEffect, &world, &deferredExample->Camera);
	//OPeffectParamVec3("vLightDirection", &deferredExample->LightDirection);
	//OPmeshRender();

	//OPrenderPresent();

	//OPframeBufferUnbind();



	//OPrenderDepth(1);
	//OPrenderClear(0, 0, 1);

	//OPbindMeshEffectWorldCam(deferredExample->Mesh, &deferredExample->Effect, &world, &deferredExample->Camera);
	//OPeffectParamVec3("vLightDirection", &deferredExample->LightDirection);
	//OPmeshRender();

	//OPtexture2DRender(deferredExample->GBuffer->Position2D);
	//OPtexture2DRender(deferredExample->GBuffer->Diffuse2D);
	//OPtexture2DRender(deferredExample->GBuffer->Normal2D);
	//OPtexture2DRender(deferredExample->GBuffer->Depth2D);

	//OPrenderPresent();

	return false;
}

void ExampleDeferredRender(OPfloat delta) {
	
}

OPint ExampleDeferredExit(OPgameState* next) {
	//OPeffectUnload(&deferredExample->Effect);
	//OPfree(deferredExample);
	return 0;
}

OPint GS_EXAMPLE_DEFERRED_AVAILABLE = 0;
OPgameState GS_EXAMPLE_DEFERRED = {
	ExampleDeferredEnter,
	ExampleDeferredUpdate,
	ExampleDeferredRender,
	ExampleDeferredExit
};