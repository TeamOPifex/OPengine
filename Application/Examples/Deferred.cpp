#include "./Examples/Deferred.h"
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/InputSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

OPgameState GS_EXAMPLE_DEFERRED = {
	ExampleDeferredEnter,
	ExampleDeferredUpdate,
	ExampleDeferredExit
};

typedef struct {
	OPtexture Position;
	OPtexture Diffuse;
	OPtexture Normal;
	OPtexture Depth;
} DeferredGBuffer;

DeferredGBuffer* DeferredGBufferCreate() {
	DeferredGBuffer* gbuffer = (DeferredGBuffer*)OPalloc(sizeof(DeferredGBuffer));

	OPtextureDescription desc = {
		OPrenderWidth, OPrenderHeight,
		GL_RGB32F, GL_RGB, GL_FLOAT,
		OPtextureLinear, OPtextureLinear,
		OPtextureClamp, OPtextureClamp
	};
	OPframeBuffer frameBuffer = OPframeBufferCreate(desc);
	OPframeBufferBind(&frameBuffer);

	gbuffer->Position = OPtextureCreate(desc);
	OPframeBufferAttach(&gbuffer->Position, 0);
	gbuffer->Diffuse = OPtextureCreate(desc);
	OPframeBufferAttach(&gbuffer->Diffuse, 1);
	gbuffer->Normal = OPtextureCreate(desc);
	OPframeBufferAttach(&gbuffer->Normal, 2);
	desc.InternalFormat = GL_DEPTH_COMPONENT32F;
	desc.Format = GL_DEPTH_COMPONENT;
	gbuffer->Depth = OPtextureCreate(desc);
	OPframeBufferAttach(&gbuffer->Depth, 3);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, DrawBuffers);

	OPframeBufferUnbind();

	return gbuffer;
}

typedef struct {
	OPmesh* Mesh;
	OPeffect Effect;
	OPcam Camera;
	ui32 Rotation;
	DeferredGBuffer* GBuffer;
	OPvec3 LightDirection;
} DeferredExample;

DeferredExample* deferredExample;

void ExampleDeferredEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	DeferredGBufferCreate();

	deferredExample = (DeferredExample*)OPalloc(sizeof(DeferredExample));

	deferredExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");

	deferredExample->Effect = OPrenderGenEffect(
		"SimpleModel.vert",
		"SimpleModel.frag",
		OPATTR_POSITION | OPATTR_NORMAL,
		"Model Effect",
		deferredExample->Mesh->VertexSize);

	deferredExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	deferredExample->LightDirection = OPvec3Create(0, 1, 0);

	OPrenderDepth(1);
}

int ExampleDeferredUpdate(OPtimer* time) {

	// Update
	if (OPkeyboardIsDown(OPKEY_P)) { deferredExample->Rotation++; }
	OPmat4 world;
	OPmat4buildRotY(&world, deferredExample->Rotation / 100.0);

	// Render
	OPrenderClear(0, 0, 0);

	OPbindMeshEffectWorldCam(deferredExample->Mesh, &deferredExample->Effect, &world, &deferredExample->Camera);
	OPrenderParamVec3("vLightDirection", &deferredExample->LightDirection);
	OPrenderMesh();

	OPrenderPresent();

	return false;
}

void ExampleDeferredExit(OPgameState* next) {
	OPrenderUnloadEffect(&deferredExample->Effect);
	OPfree(deferredExample);
}
