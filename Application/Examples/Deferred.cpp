#include "./Examples/Deferred.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Input/Input.h"

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
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
	DeferredGBuffer* GBuffer;
} DeferredExample;

DeferredExample* deferredExample;

void ExampleDeferredEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	DeferredGBufferCreate();

	deferredExample = (DeferredExample*)OPalloc(sizeof(DeferredExample));

	deferredExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	deferredExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*deferredExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		2,
		"Deferred Effect",
		deferredExample->Mesh->VertexSize
		);

	deferredExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*deferredExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);
}

int ExampleDeferredUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardIsDown(OPKEY_P)) { deferredExample->Rotation++; }

	OPrenderBindMesh(deferredExample->Mesh);
	OPrenderBindEffect(deferredExample->Effect);

	OPmat4 world, view, proj;
	OPmat4buildRotY(&world, deferredExample->Rotation / 100.0);

	OPcamGetView((*deferredExample->Camera), &view);
	OPcamGetProj((*deferredExample->Camera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleDeferredExit(OPgameState* next) {
	OPfree(deferredExample->Effect);
	OPfree(deferredExample->Camera);

	OPfree(deferredExample);
}
