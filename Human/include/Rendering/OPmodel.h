#pragma once

struct OPmodel;
typedef struct OPmodel OPmodel;

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPboundingBox3D.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Data/include/OPcman.h"

struct OPmodel {
	OPmesh* meshes = NULL;
	OPuint meshCount = 0;
	OPchar* name = NULL;

	OPvertexLayout vertexLayout;
	OPvertexArray vertexArray;
	OPvertexBuffer vertexBuffer;
	OPindexBuffer indexBuffer;
	bool indexed = true;

	OPboundingBox3D bounds;

	OPmodel() { }
	OPmodel(OPuint meshCount, OPvertexLayout vertexLayout) { Init(meshCount, vertexLayout); }

	void Init(OPuint meshCount, OPvertexLayout vertexLayout);
	void Build(ui32 vertexCount, void* vertices);
	void Build(ui32 vertexCount, ui32 indexCount, OPindexSize::Enum indexSize, void* vertices, void* indices);
	void Bind();
	void Draw(OPmat4* world, OPmaterial* material, OPcam* camera);
	void Destroy();
	void Update(ui32 vertexCount, void* vertices);

    inline void Free() {
        Destroy();
        OPfree(this);
    }

	inline static OPmodel* Create(OPuint meshCount, OPvertexLayout vertexLayout) {
		return OPNEW(OPmodel(meshCount, vertexLayout));
	}

	inline static OPmodel* Load(const OPchar* name) {
		return (OPmodel*)OPCMAN.LoadGet(name);
	}
};
