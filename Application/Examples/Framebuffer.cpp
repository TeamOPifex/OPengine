#include "./ExampleSelectorState.h"

#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

class FrameBufferExample : public OPgameState {
	OPframeBuffer frameBuffer;
	OPtexture2DOLD* texture2D;

	OPmodel Mesh;
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui64 Rotation;			// The amount to rotate the Mesh


	void Init(OPgameState* last) {
		OPtextureDesc textureDesc = OPtextureDesc(512, 512, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST);
		frameBuffer.Init(textureDesc);
		texture2D = OPtexture2DCreate(&frameBuffer.texture[0]);


		Effect.Init("ColoredModel.vert", "ColoredModel.frag");
		Mesh = *(OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
		Camera.SetPerspective(OPVEC3_ONE * 20.0, OPVEC3_ZERO, OPVEC3_UP, 0.1f, 1000.0f, 45.0f, 1.0f);
	}

	OPint Update(OPtimer* time) {

		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation += time->Elapsed; }

		frameBuffer.Bind();
		OPrenderClear(0.0, 0, 0);

		OPrenderDepth(1);
		OPrenderCull(0);

		OPmat4 world;
		world.SetRotY(Rotation / 100.0f)->Scl(0.25f, 0.25f, 0.25f);
		OPbindMeshEffectWorldCam(&Mesh, &Effect, &world, &Camera);
		OPrenderDrawBufferIndexed(0);
		frameBuffer.Unbind();

		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0.1);
		OPtexture2DRender(texture2D);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		OPtexture2DUnloadGlobals();
		Effect.Destroy();
		return 0;
	}
};



OPint GS_EXAMPLE_FRAMEBUFFER_AVAILABLE = 1;
FrameBufferExample _GS_EXAMPLE_FRAMEBUFFER;
OPgameState* GS_EXAMPLE_FRAMEBUFFER = &_GS_EXAMPLE_FRAMEBUFFER;