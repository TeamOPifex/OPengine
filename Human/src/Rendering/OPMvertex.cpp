#include "./Human/include/Rendering/OPMvertex.h"
ui32 OPMhasFeature(ui32 features, OPMFeatures feature) {
	return (features & feature) > 0;
}

OPvertices* OPverticesCreate(OPuint count, ui32 features) {
	ui32 size = 0;
	size += OPMhasFeature(features, Position) * 3;
	size += OPMhasFeature(features, Normal) * 3;
	size += OPMhasFeature(features, Tangent) * 3;
	size += OPMhasFeature(features, UV) * 2;
	size += OPMhasFeature(features, Bones) * 4;
	size += OPMhasFeature(features, Skinning) * 4;
	size += OPMhasFeature(features, Color) * 3;
	//size *= sizeof(f32);

	OPvertices* vertices = (OPvertices*)OPalloc(sizeof(OPvertices));
	OPuint totalDataSize = size * sizeof(f32)* count;
	vertices->data = (i8*)OPallocZero(totalDataSize);
	vertices->features = features;
	vertices->size = size;
	vertices->count = count;
	return vertices;
}

ui32 OPverticesOffset(OPvertices* vertices, ui32 feature) {
	ui32 offset = 0;

	if (feature & Position) return offset;
	offset += OPMhasFeature(vertices->features, Position) * 3;

	if (feature & Normal) return offset;
	offset += OPMhasFeature(vertices->features, Normal) * 3;
	
	if (feature & Tangent) return offset;
	offset += OPMhasFeature(vertices->features, Tangent) * 3;

	if (feature & UV) return offset;
	offset += OPMhasFeature(vertices->features, UV) * 2;

	if (feature & Bones) return offset;
	offset += OPMhasFeature(vertices->features, Bones) * 4;

	if (feature & Skinning) return offset;
	offset += OPMhasFeature(vertices->features, Skinning) * 4;

	if (feature & Color) return offset;
	offset += OPMhasFeature(vertices->features, Color) * 3;

	return offset;
}

void OPverticesWriteVec2(OPvertices* vertices, OPvec2* data, OPMFeatures feature) {
	if (!OPMhasFeature(vertices->features, feature)) return;

	ui32 offset = OPverticesOffset(vertices, feature);
	for (ui32 i = 0; i < vertices->count; i++) {
		void* dst = &vertices->data[offset * sizeof(f32)];
		void* src = &data[i];
		offset += vertices->size;
		OPmemcpy(dst, src, sizeof(OPvec2));
	}
}

void OPverticesWriteVec3(OPvertices* vertices, OPvec3* data, OPMFeatures feature) {
	if (!OPMhasFeature(vertices->features, feature)) return;

	ui32 offset = OPverticesOffset(vertices, feature);
	for (ui32 i = 0; i < vertices->count; i++) {
		void* dst = &vertices->data[offset * sizeof(f32)];
		void* src = &data[i];
		OPmemcpy(dst, src, sizeof(OPvec3));
		offset += vertices->size;
	}
}

void OPverticesWriteVec4(OPvertices* vertices, OPvec4* data, OPMFeatures feature) {
	if (!OPMhasFeature(vertices->features, feature)) return;

	ui32 offset = OPverticesOffset(vertices, feature);
	for (ui32 i = 0; i < vertices->count; i++) {
		void* dst = &vertices->data[offset * sizeof(f32)];
		void* src = &data[i];
		offset += vertices->size;
		OPmemcpy(dst, src, sizeof(OPvec4));
	}
}
