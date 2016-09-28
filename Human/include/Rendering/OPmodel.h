#pragma once

struct OPmodel;
typedef struct OPmodel OPmodel;

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Data/include/OPcman.h"

struct OPmodel {
	OPmesh* meshes;
	OPuint meshCount;
	OPchar* name;

	OPvertexLayout vertexLayout;
	OPvertexArray vertexArray;
	OPvertexBuffer vertexBuffer;
	OPindexBuffer indexBuffer;

	OPmodel() { }
	OPmodel(OPuint meshCount, OPvertexLayout vertexLayout) { Init(meshCount, vertexLayout); }
	
	void Init(OPuint meshCount, OPvertexLayout vertexLayout);
	void Build(ui32 vertexCount, ui32 indexCount, OPindexSize indexSize, void* vertices, void* indices);
	void Bind();
	void Draw(OPmat4* world, OPmaterial* material, OPcam* camera);
	void Destroy();

	inline static OPmodel* Create(OPuint meshCount, OPvertexLayout vertexLayout) {
		return OPNEW(OPmodel(meshCount, vertexLayout));
	}
};