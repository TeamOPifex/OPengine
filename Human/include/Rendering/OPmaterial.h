#ifndef OPENGINE_HUMAN_RENDERER_MATERIAL
#define OPENGINE_HUMAN_RENDERER_MATERIAL

#include "OPeffect.h"
#include "./Core/include/OPtypes.h"

enum OPmaterialParamType {
	MATERIAL_PARAM_TYPE_MATRIX4,
	MATERIAL_PARAM_TYPE_MATRIX4V,
	MATERIAL_PARAM_TYPE_TEXTURE,
	MATERIAL_PARAM_TYPE_VECTOR3,
	MATERIAL_PARAM_TYPE_VECTOR4,
	MATERIAL_PARAM_TYPE_FLOAT
};

struct OPmaterialParam {
	OPmaterialParamType  type;
	const OPchar*        name;
	void*                data;
	ui8                  count;
};
typedef struct OPmaterialParam OPmaterialParam;

struct OPmaterial;

inline void OPmaterialClearParams(OPmaterial* material);
void OPmaterialInit(OPmaterial* material, OPeffect* effect);
inline OPmaterial* OPmaterialCreate(OPeffect* effect);
inline void OPmaterialAddParam(OPmaterial* material, OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count );
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtexture* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec3* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec4* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data, ui8 count);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data);
inline void OPmaterialBind(OPmaterial* material);
inline void OPmaterialBind(OPmaterial* material, ui32 stride);

struct OPmaterial {
	OPeffect* effect;
	OPmaterialParam params[10];
	OPuint paramIndex;
	ui64 id;
	i8 depth;
	i8 cull;
	i8 alpha;

	void SetDepth(i8 val) {
		depth = val;
	}

	void SetCull(i8 val) {
		cull = val;
	}

	void ClearParams() {
	    OPmaterialClearParams(this);
	}

	void Init(OPeffect* effect) {
	    OPmaterialInit(this, effect);
	}

	void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
	    OPmaterialAddParam(this, paramType, name, data, count);
	}

	void AddParam(const OPchar* name, OPtexture* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPvec3* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPvec4* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPmat4* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
	    OPmaterialAddParam(this, name, data, count);
	}

	void AddParam(const OPchar* name, f32* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void Bind() {
	    OPmaterialBind(this);
	}
};
typedef struct OPmaterial OPmaterial;

inline void OPmaterialClearParams(OPmaterial* material) {
	material->paramIndex = 0;
}

inline OPmaterial* OPmaterialCreate(OPeffect* effect) {
	OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
	OPmaterialInit(material, effect);
	return material;
}

inline void OPmaterialAddParam(OPmaterial* material, OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count ) {
	OPlog("Name %s %p", name, data);
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

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_FLOAT, name, (void*)data, 1);
}

inline void OPmaterialBind(OPmaterial* material) {
	OPeffectBind(material->effect);

	OPrenderDepth(material->depth);
	OPrenderCull(material->cull);

	//OPtextureClearActive();
	for(OPuint i = 0; i < material->paramIndex; i++) {

		switch(material->params[i].type) {
			case MATERIAL_PARAM_TYPE_TEXTURE: {
				OPeffectParam(material->params[i].name, (OPtexture*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR3: {
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
			case MATERIAL_PARAM_TYPE_FLOAT: {
				OPeffectParam(material->params[i].name, *(f32*)material->params[i].data);
				break;
			}
		}
	}
}

inline void OPmaterialBind(OPmaterial* material, ui32 stride) {
	OPeffectBind(material->effect, stride);

	OPrenderDepth(material->depth);
	OPrenderCull(material->cull);

	//OPtextureClearActive();
	for(OPuint i = 0; i < material->paramIndex; i++) {

		switch(material->params[i].type) {
			case MATERIAL_PARAM_TYPE_TEXTURE: {
				OPeffectParam(material->params[i].name, (OPtexture*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR3: {
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
			case MATERIAL_PARAM_TYPE_FLOAT: {
				OPeffectParam(material->params[i].name, *(f32*)material->params[i].data);
				break;
			}
		}
	}
}


#endif
