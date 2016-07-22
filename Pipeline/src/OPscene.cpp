#include "./Pipeline/include/OPscene.h"

void OPscene::Init(ui32 initialCount) {
	count = initialCount;
	index = 0;
	entities = (OPscenePair*)OPalloc(sizeof(OPscenePair) * count);

	camera.SetPerspective(OPvec3(2), OPvec3(0));

	allocator = OPallocatorLinearCreate(KB(1) * initialCount);
	renderBucket.Init(initialCount, &camera, allocator);
}

OPscenePair* OPscene::Add(OPmodel* model, OPmaterial* material) {
	entities[index].model = model;
	entities[index].material = material;
	return &entities[index++];
}

OPint OPscene::Update(OPtimer* timer) {
	return 0;
}

void OPscene::Render(OPfloat delta) {

	for (ui32 i = 0; i < index; i++) {
		OPcommandDrawIndexed* dc = renderBucket.CreateDrawIndexed();
		dc->Set(entities[i].model, entities[i].material);
		renderBucket.Submit(dc->key, dc->dispatch, dc);
	}

	renderBucket.Sort();
	renderBucket.Flush();

}