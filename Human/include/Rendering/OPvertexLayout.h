#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT
#define OPENGINE_HUMAN_RENDERING_VERTEX_LAYOUT

#include "./Human/include/Rendering/OPshader.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Human/include/Rendering/OPattributes.h"

struct OPvertexLayout {
    ui16 count;
	OPshaderAttribute* attributes;
    ui32 stride;
};
typedef struct OPvertexLayout OPvertexLayout;

inline void OPvertexLayoutInit(OPvertexLayout* layout, ui16 count, OPchar** names, OPattributeTypes* types, ui8* counts) {

	layout->count = count;
    layout->attributes = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute) * count);

	ui32 offset = 0;
	layout->stride = 0;
	f32 attributeSize;
    for(ui16 i = 0 ; i < count; i++ ) {
		layout->attributes[i].Name = names[i];
		layout->attributes[i].Type = GL_FLOAT;// types[i];
		layout->attributes[i].Elements = counts[i];
		layout->attributes[i].Offset = offset;
		attributeSize = counts[i] * sizeof(f32);
		offset += attributeSize;
		layout->stride += attributeSize;
    }
}
inline void OPvertexLayoutInit(OPvertexLayout* layout, OPshaderAttribute* attributes, ui16 count) {
	layout->count = count;
    layout->attributes = attributes;

	layout->stride = 0;
    for(ui16 i = 0 ; i < count; i++ ) {
		layout->stride += layout->attributes[i].Elements * sizeof(f32);
    }
}



#define OPVERTEX_LAYOUT_BUILDER_MAX 32

struct OPvertexLayoutBuilder;
inline void OPvertexLayoutBuilderInit(OPvertexLayoutBuilder* builder);
inline OPvertexLayout OPvertexLayoutBuilderBuild(OPvertexLayoutBuilder* builder);

struct OPvertexLayoutBuilder {
    ui32 index;
    OPchar* names[OPVERTEX_LAYOUT_BUILDER_MAX]; // Assuming we'll never need more than 32 attributes in a vertex layout
	OPattributeTypes types[OPVERTEX_LAYOUT_BUILDER_MAX];
	ui8 counts[OPVERTEX_LAYOUT_BUILDER_MAX];

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

inline void OPvertexLayoutBuilderAdd(OPvertexLayoutBuilder* builder, OPattributes attribute) {
	switch (attribute) {
		case OPATTR_POSITION: {
			builder->names[builder->index] = "aPosition";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 3;
			builder->index++;
			break;
		}
		case OPATTR_NORMAL: {
			builder->names[builder->index] = "aNormal";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 3;
			builder->index++;
			break;
		}
		case OPATTR_TANGENT: {
			builder->names[builder->index] = "aTangent";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 3;
			builder->index++;
			break;
		}
		case OPATTR_COLOR: {
			builder->names[builder->index] = "aColor";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 3;
			builder->index++;
			break;
		}
		case OPATTR_UV: {
			builder->names[builder->index] = "aUV";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 2;
			builder->index++;
			break;
		}
		case OPATTR_BONES: {
			builder->names[builder->index] = "aBones";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 4;
			builder->index++;
			builder->names[builder->index] = "aWeights";
			builder->types[builder->index] = OPATTR_TYPE_FLOAT;
			builder->counts[builder->index] = 4;
			builder->index++;
			break;
		}
	}
}

inline OPvertexLayout OPvertexLayoutBuilderBuild(OPvertexLayoutBuilder* builder) {
    OPvertexLayout result;
    OPvertexLayoutInit(&result, builder->index, builder->names, builder->types, builder->counts);
    return result;
}


#endif
