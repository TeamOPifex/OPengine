#include "./ExampleSelectorState.h"

#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

typedef struct {
	OPframeBuffer frameBuffer;
	OPtexture2DOLD* texture2D;

	OPmodel Mesh;
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui64 Rotation;			// The amount to rotate the Mesh
} FrameBufferExample;

FrameBufferExample frameBufferExample;

void ExampleFrameBufferEnter(OPgameState* last) {
	OPtextureDesc textureDesc = OPtextureDesc(512, 512, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST);
	frameBufferExample.frameBuffer.Init(textureDesc);
	frameBufferExample.texture2D = OPtexture2DCreate(&frameBufferExample.frameBuffer.texture);


	frameBufferExample.Effect.Init("ColoredModel.vert", "ColoredModel.frag");
	frameBufferExample.Mesh = *(OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
	frameBufferExample.Camera.SetPerspective(OPVEC3_ONE * 20.0, OPVEC3_ZERO, OPVEC3_UP, 0.1f, 1000.0f, 45.0f, 1.0f);
}

OPint ExampleFrameBufferUpdate(OPtimer* time) {

	if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { frameBufferExample.Rotation += time->Elapsed; }

	frameBufferExample.frameBuffer.Bind();
	OPrenderClear(0.0, 0, 0);

	OPrenderDepth(1);
	OPrenderCull(0);

	OPmat4 world;
	world.SetRotY(frameBufferExample.Rotation / 100.0f)->Scl(0.25f, 0.25f, 0.25f);
	OPbindMeshEffectWorldCam(&frameBufferExample.Mesh, &frameBufferExample.Effect, &world, &frameBufferExample.Camera);
	OPrenderDrawBufferIndexed(0);
	frameBufferExample.frameBuffer.Unbind();

	return false;
}

void ExampleFrameBufferRender(OPfloat delta) {
	OPrenderClear(0.1);
	OPtexture2DRender(frameBufferExample.texture2D);
	OPrenderPresent();
}

OPint ExampleFrameBufferExit(OPgameState* next) {
	OPtexture2DUnloadGlobals();
	frameBufferExample.Effect.Destroy();
	return 0;
}

OPint GS_EXAMPLE_FRAMEBUFFER_AVAILABLE = 1;
OPgameState GS_EXAMPLE_FRAMEBUFFER = {
	ExampleFrameBufferEnter,
	ExampleFrameBufferUpdate,
	ExampleFrameBufferRender,
	ExampleFrameBufferExit
};
