#include "./EditorState.h"
#include "./OPcef.h"

struct Grid {
	OPmesh mesh;
	OPeffect effect;

	void Init(ui32 width, ui32 depth) {
		effect = OPeffectGen("Grid.vert", "Grid.frag", OPATTR_POSITION, "Grid Effect", sizeof(OPvec3));

		ui32 totalVertices = width * depth;

		OPvec3* vertices = (OPvec3*)OPalloc(sizeof(OPvec3) * totalVertices);
		f32 halfWidth = width / 2.0f;
		f32 halfDepth = depth / 2.0f;
		for (ui32 i = 0; i < width; i++) {
			for (ui32 j = 0; j < depth; j++) {
				OPvec3* point = &vertices[j * width + i];
				point->x = j - halfWidth;
				point->y = 0;
				point->z = i - halfDepth;
			}
		}

		ui32 totalIndices = 2 * (width - 1) * depth * 2;
		ui16* indices = (ui16*)OPalloc(sizeof(ui16) * totalIndices);

		ui32 i = 0;
		for (ui32 y = 0; y < width; y++) {
			for (ui32 x = 0; x < depth - 1; x++) {
				indices[i++] = y * width + x;
				indices[i++] = y * width + x + 1;
			}
		}

		// Vertical grid lines
		for (ui32 x = 0; x < width; x++) {
			for (ui32 y = 0; y < depth - 1; y++) {
				indices[i++] = y * width + x;
				indices[i++] = (y + 1) * width + x;
			}
		}

		mesh = OPmeshCreate();
		OPmeshBind(&mesh);
		OPmeshBuild(sizeof(OPvec3), sizeof(ui16), totalVertices, totalIndices, vertices, indices);
	}
};

Grid grid;
OPcamFreeFlight camera;

void EditorEnter(OPgameState* prev) {
	grid.Init(11, 11);
	camera.Init();
	
	return;
}

OPint EditorUpdate(OPtimer* timer) {
	camera.Update(timer);
	return 0;
}

void EditorRender(OPfloat delta) {
	OPrenderClear(0.1, 0.1, 0.1);

	OPmeshBind(&grid.mesh);
	OPeffectBind(&grid.effect);
	OPmeshBind(&grid.mesh);
	OPeffectParamMat4("uWorld", &OPMAT4_IDENTITY);
	OPeffectParamMat4("uView", camera.View());
	OPeffectParamMat4("uProj", camera.Proj());
	glDrawElements(GL_LINES, grid.mesh.IndexBuffer.ElementCount, GL_UNSIGNED_SHORT, 0);

	OPcefUpdate();

	OPrenderPresent();
}

OPint EditorExit(OPgameState* next) {
	return 1;
}

OPgameState GS_EDITOR = {
	EditorEnter,
	EditorUpdate,
	EditorRender,
	EditorExit
};
