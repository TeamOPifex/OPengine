#ifndef OPENGINE_PIPELINE_SCENE
#define OPENGINE_PIPELINE_SCENE

struct OPscene;
typedef struct OPscene OPscene;

#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPcommandBucket.h"

struct OPscenePair {
	OPmodel* model;
	OPmaterial* material;
};

struct OPscene {
	OPcam camera;
	OPscenePair* entities;
	ui32 count;
	ui32 index;
	OPcommandBucket renderBucket;
	OPallocator* allocator;

	void Init(ui32 initialCount);
	OPscenePair* Add(OPmodel* model, OPmaterial* material);
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
};


#endif
