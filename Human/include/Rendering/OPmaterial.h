#ifndef OPENGINE_HUMAN_RENDERER_MATERIAL
#define OPENGINE_HUMAN_RENDERER_MATERIAL

#include "OPeffect.h"
#include "./Core/include/OPtypes.h"	

enum OPmaterialParamType {
	MATERIAL_PARAM_TYPE_MATRIX4,
	MATERIAL_PARAM_TYPE_MATRIX4V,
	MATERIAL_PARAM_TYPE_TEXTURE,
	MATERIAL_PARAM_TYPE_VECTOR3,
	MATERIAL_PARAM_TYPE_VECTOR4
};

typedef struct {
	OPmaterialParamType  type;
	const OPchar*        name;
	void*                data;
	ui8                  count;
} OPmaterialParam;

typedef struct {
	OPeffect* effect;
	OPmaterialParam params[10];
	OPuint paramIndex;
} OPmaterial;

inline void OPmaterialInit(OPmaterial* material, OPeffect* effect) {
	material->effect = effect;
	material->paramIndex = 0;
}

inline OPmaterial* OPmaterialCreate(OPeffect* effect) {
	OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
	OPmaterialInit(material, effect);
	return material;
}

inline void OPmaterialAddParam(OPmaterial* material, OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count ) {
	OPlog("Name %s", name);
	material->params[material->paramIndex].type = paramType;
	material->params[material->paramIndex].name = name;
	material->params[material->paramIndex].data = data;
	material->params[material->paramIndex].count = count;
	material->paramIndex++;
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtexture* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_TEXTURE, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec3* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_VECTOR3, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec4* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_VECTOR4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_MATRIX4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data, ui8 count) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_MATRIX4V, name, (void*)data, count);
}

inline void OPmaterialBind(OPmaterial* material) {
	OPeffectBind(material->effect);

	OPtextureClearActive();
	OPlog("Param Count %d", material->paramIndex);
	for(OPuint i = 0; i < material->paramIndex; i++) {
		switch(material->params[i].type) {
			case MATERIAL_PARAM_TYPE_TEXTURE: {
				OPeffectParam(material->params[i].name, (OPtexture*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR3: {
				OPvec3Log("COLOR", (*(OPvec3*)material->params[i].data));
				OPeffectParam(material->params[i].name, *(OPvec3*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR4: {
				OPeffectParam(material->params[i].name, *(OPvec4*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_MATRIX4: {
				OPeffectParam(material->params[i].name, *(OPmat4*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_MATRIX4V: {
				OPeffectParam(material->params[i].name, material->params[i].count, (OPmat4*)material->params[i].data);
				break;
			}
		}
	}
}


#endif