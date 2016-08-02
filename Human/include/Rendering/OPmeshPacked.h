#ifndef OPENGINE_HUMAN_RENDERER_MESH_PACKED
#define OPENGINE_HUMAN_RENDERER_MESH_PACKED

#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/Enums/OPindexSize.h"
#include "./Human/include/Rendering/OPvertexLayout.h"

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPmeshPacked {
	OPuint offset;
	OPuint count;
	OPuint elementCount;
	OPvertexLayout vertexLayout;

	OPmeshPacked() {

	}

	OPmeshPacked(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices) {
		Init(vertexLayout, indSize, vertCount, indCount, vertices, indices);
	}
	OPmeshPacked* Init(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
	static OPmeshPacked* Create(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
	void Render();
};
typedef struct OPmeshPacked OPmeshPacked;


#endif
