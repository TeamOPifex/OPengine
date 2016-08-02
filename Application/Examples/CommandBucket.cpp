#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPcommandBucket.h"

#include <bitset>
#include <string>

struct CommandBucketExample {
	OPmodel model;
	OPmodel model2;
	OPeffect effect;		// The Effect used to render the Mesh
	OPmaterial material;
	OPcam camera;			// The Camera to use in the Effect to render the Mesh
	ui32 rotation;			// The amount to rotate the Mesh
	OPvec3 lightDirection;	// Where the Light Source is coming from
	OPcommandBucket renderBucket;
	OPallocator* allocator;

	void Init(OPgameState* last) {
		allocator = OPallocatorLinearCreate(MB(4));

    	model.Init("output.opm");
    	model2.Init("patrick.opm");

		effect.Init("ColoredModel.vert", "ColoredModel.frag");

    	camera.SetPerspective(
    		OPVEC3_ONE * 2.0, OPVEC3_UP, OPVEC3_UP,
    		0.1f, 1000.0f,
    		45.0f, OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
    		);

    	lightDirection = OPVEC3_UP;

        material.Init(&effect);
        material.AddParam("vLightDirection", &lightDirection);

    	OPrenderDepth(1);
    	OPrenderCull(0);

    	renderBucket.Init(16, &camera, allocator);
	}

	OPint Update(OPtimer* time) {
	    if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { rotation++; }

    	model.world.SetRotY(rotation / 100.0f);
    	model.world.Scl(0.25f);
    	model2.world = OPmat4Translate(1, 0, 0);
    	model2.world.Scl(0.025f);

    	return false;
	}

	void Render(OPfloat delta) {
    	OPrenderClear(0.4f, 0.4f, 0.4f);

    	OPcommandDrawIndexed* dc = renderBucket.CreateDrawIndexed();
    	dc->Set(&model2, &material);
    	renderBucket.Submit(dc->key, dc->dispatch, dc);

    	dc = renderBucket.CreateDrawIndexed();
    	dc->Set(&model, &material);
    	renderBucket.Submit(dc->key, dc->dispatch, dc);

        renderBucket.Sort();
        renderBucket.Flush();

    	OPrenderPresent();
    }

    OPint Exit(OPgameState* next) {
    	//OPeffectUnload(&effect);
    	return 0;
    }
};

// The actual game state state
CommandBucketExample commandBucketExample;


// Wrapping methods
void ExampleCommandBucketEnter(OPgameState* last) {
    commandBucketExample.Init(last);
}
OPint ExampleCommandBucketUpdate(OPtimer* time) {
    return commandBucketExample.Update(time);
}
void ExampleCommandBucketRender(OPfloat delta) {
    commandBucketExample.Render(delta);
}
OPint ExampleCommandBucketExit(OPgameState* next) {
    return commandBucketExample.Exit(next);
}

OPint GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE = 1;
OPgameState GS_EXAMPLE_COMMAND_BUCKET = {
	ExampleCommandBucketEnter,
	ExampleCommandBucketUpdate,
	ExampleCommandBucketRender,
	ExampleCommandBucketExit
};
