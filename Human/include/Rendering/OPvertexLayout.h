#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT
#define OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT

#include "./Human/include/Rendering/OPshader.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Human/include/Rendering/OPattributes.h"


struct OPvertexLayout;
typedef struct OPvertexLayout OPvertexLayout;

struct OPeffect;
struct OPvertexLayout {
	ui16 count;
	OPshaderAttribute* attributes;
	ui32 stride;

	void Init(ui16 attributeCount, OPchar** names, OPattributeTypes* types, ui8* counts);
	void Init(OPshaderAttribute* attributes, ui16 count);
	void SetOffsets(OPeffect* effect);
};


// Assuming we'll never need more than 32 attributes in a vertex layout
#define OPVERTEX_LAYOUT_BUILDER_MAX 32

struct OPvertexLayoutBuilder;
typedef struct OPvertexLayoutBuilder OPvertexLayoutBuilder;

struct OPvertexLayoutBuilder {
    ui32 index;
    OPchar* names[OPVERTEX_LAYOUT_BUILDER_MAX];
	OPattributeTypes types[OPVERTEX_LAYOUT_BUILDER_MAX];
	ui8 counts[OPVERTEX_LAYOUT_BUILDER_MAX];

	void Init();
	void Init(ui32 features);
	void Add(OPattributes attribute);
	OPvertexLayout Build();
};




#endif
