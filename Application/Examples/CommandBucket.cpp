#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPcommandBucket.h"

#include <bitset>
#include <string>

// Data for this Game State Example
typedef struct {
	OPmodel Model;
	OPmodel Model2;
	OPeffect Effect;		// The Effect used to render the Mesh
	OPmaterial Material;
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
	OPcommandBucket RenderBucket;
} CommandBucketExample;

CommandBucketExample commandBucketExample;

ui64 KeyGen(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material) {
    
    ui64 meshId = model->mesh->Id << 0;                   // 00 - 06 bits
    ui64 materialId = material->id << 6;    // 07 - 12 bits
    ui64 renderTarget = 0 << 12;            // 13 - 19 bits
    
    ui64 result = meshId | materialId | renderTarget;
    
    // std::bitset<64> bitset1 { result };   // the bitset representation of 4
    // std::string str = bitset1.to_string();
    // OPlogDebug("Bitset %s", str.c_str());
    // OPlogDebug("MaterialId %u", material->id);
    // OPlogDebug("MaterialId %u", materialId);
    // OPlogDebug("meshId %u", model->mesh->Id);
    // OPlogDebug("meshId %u", meshId);
    // OPlogDebug("Result %u", result);
    
    return result;
}

void ExampleCommandBucketEnter(OPgameState* last) {

	commandBucketExample.Model.Init("output.opm");
	commandBucketExample.Model2.Init("patrick.opm");

	commandBucketExample.Effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		commandBucketExample.Model.mesh->VertexSize);

	commandBucketExample.Camera = OPcamPersp(
		OPVEC3_ONE * 2.0, OPVEC3_UP, OPVEC3_UP,
		0.1f, 1000.0f,
		45.0f, OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	commandBucketExample.LightDirection = OPVEC3_UP;

    commandBucketExample.Material.Init(&commandBucketExample.Effect);
    commandBucketExample.Material.AddParam("vLightDirection", &commandBucketExample.LightDirection);

	OPrenderDepth(1);
	OPrenderCull(0);
	
	commandBucketExample.RenderBucket.Init(KeyGen, 16, &commandBucketExample.Camera);
	OPlogDebug("TEST KEY INDEX: %u", commandBucketExample.RenderBucket.keyIndex);
}

OPint ExampleCommandBucketUpdate(OPtimer* time) {

	if (OPkeyboardIsDown(OPKEY_SPACE)) { commandBucketExample.Rotation++; }

	commandBucketExample.Model.world.SetRotY(commandBucketExample.Rotation / 100.0);
	commandBucketExample.Model.world.Scl(0.25f);
	commandBucketExample.Model2.world = OPmat4Translate(1, 0, 0);
	commandBucketExample.Model2.world.Scl(0.025f);

	return false;
}

void ExampleCommandBucketRender(OPfloat delta) {
	OPrenderClear(0.4, 0.4, 0.4);
	
	commandBucketExample.RenderBucket.DrawIndexed(&commandBucketExample.Model2, &commandBucketExample.Material);
	commandBucketExample.RenderBucket.DrawIndexed(&commandBucketExample.Model, &commandBucketExample.Material);
    commandBucketExample.RenderBucket.Sort();
    commandBucketExample.RenderBucket.Flush();
    
	OPrenderPresent();
}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleCommandBucketExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPeffectUnload(&commandBucketExample.Effect);
	return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_COMMAND_BUCKET = {
	ExampleCommandBucketEnter,
	ExampleCommandBucketUpdate,
	ExampleCommandBucketRender,
	ExampleCommandBucketExit
};
