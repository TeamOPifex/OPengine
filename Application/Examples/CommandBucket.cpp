#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"

#include <bitset>
#include <string>

class CommandBucketExample : public OPgameState {
	OPmodel* model;
	OPmodel* model2;
	OPmat4 world1, world2;
	OPeffect effect;		// The Effect used to render the Mesh
	OPmaterial material;
	OPmaterial materialInstance;
	OPcam camera;			// The Camera to use in the Effect to render the Mesh
	OPcam* cameraPtr;
	ui32 rotation;			// The amount to rotate the Mesh
	OPvec3 lightDirection;	// Where the Light Source is coming from
	OPrenderCommandBucket renderBucket;
	OPallocator* allocator;
	OPallocatorLinear* linearAllocator;

	void Init(OPgameState* last) {
		linearAllocator = OPallocatorLinear::Create(MB(4));
		allocator = linearAllocator->GetAllocator();

		model = (OPmodel*)OPCMAN.LoadGet("output.opm");
		model2 = (OPmodel*)OPCMAN.LoadGet("patrick.opm");

		effect.Init("ColoredModel.vert", "ColoredModel.frag");

    	camera.SetPerspective(
    		OPVEC3_ONE * 2.0, OPVEC3_UP, OPVEC3_UP,
    		0.1f, 1000.0f,
    		45.0f, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
    		);

    	lightDirection = OPVEC3_UP;

        material.Init(&effect);
        material.AddParam("vLightDirection", &lightDirection);
		materialInstance.Init(&material);

    	OPrenderDepth(1);
    	OPrenderCull(0);

		cameraPtr = &camera;
    	renderBucket.Init(16, &cameraPtr, allocator);
	}

	OPint Update(OPtimer* time) {
	    if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { rotation++; }

    	world1.SetRotY(rotation / 100.0f);
    	world1.Scl(0.25f);
		world2 = OPmat4Translate(1, 0, 0);
		world2.Scl(0.025f);

    	return false;
	}

	void Render(OPfloat delta) {
    	OPrenderClear(0.4f, 0.4f, 0.4f);

		renderBucket.Submit(model2, &world2, &materialInstance);
		renderBucket.Submit(model, &world1, &materialInstance);

        renderBucket.Sort();
        renderBucket.Flush();

    	OPrenderPresent();
    }

    OPint Exit(OPgameState* next) {
    	//OPeffectUnload(&effect);
    	return 0;
    }
};
OPint GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE = 1;
CommandBucketExample _GS_EXAMPLE_COMMAND_BUCKET;
OPgameState* GS_EXAMPLE_COMMAND_BUCKET = &_GS_EXAMPLE_COMMAND_BUCKET;