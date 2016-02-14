#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT
#define OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT

#include "./Data/include/OPstring.h"

struct OPvertexLayout {
    ui16 count;
    OPchar** attributes;
    ui32* offsets;
    ui32 stride;
    
    ui32 Size() {
        return stride;
    }
};
typedef struct OPvertexLayout OPvertexLayout;

inline void OPvertexLayoutInit(OPvertexLayout* layout, ui16 count, OPchar** names, ui32* size) {
    OPuint nameSize = sizeof(OPchar*) * count;
    OPuint offsetSize = sizeof(ui32) * count;
    void* memory = OPalloc(nameSize + offsetSize);
    
    layout->attributes = (OPchar**)memory;
    layout->offsets = (ui32*)( (OPint)memory + nameSize );
    
    for(ui16 i = 0 ; i < count; i++ ) {
        layout->attributes[i] = names[i];
        layout->offsets[i] = size[i];
        layout->stride += size[i];
    }
}



#define OPVERTEX_LAYOUT_BUILDER_MAX 32

struct OPvertexLayoutBuilder;
inline void OPvertexLayoutBuilderInit(OPvertexLayoutBuilder* builder);
inline OPvertexLayout OPvertexLayoutBuilderBuild(OPvertexLayoutBuilder* builder);

struct OPvertexLayoutBuilder {
    ui32 index;
    OPchar* names[OPVERTEX_LAYOUT_BUILDER_MAX]; // Assuming we'll never need more than 32 attributes in a vertex layout
    ui32 sizes[OPVERTEX_LAYOUT_BUILDER_MAX];
    
    void Init() {
        OPvertexLayoutBuilderInit(this);
    }
    
    OPvertexLayout Build() {
        return OPvertexLayoutBuilderBuild(this);
    }
};
typedef struct OPvertexLayoutBuilder OPvertexLayoutBuilder;

inline void OPvertexLayoutBuilderInit(OPvertexLayoutBuilder* builder) {
    builder->index = 0;
}

inline OPvertexLayout OPvertexLayoutBuilderBuild(OPvertexLayoutBuilder* builder) {
    OPvertexLayout result;
    OPvertexLayoutInit(&result, builder->index, builder->names, builder->sizes);
    return result;
}


#endif