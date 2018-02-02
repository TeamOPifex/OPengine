#pragma once

struct OPeffect;
typedef struct OPeffect OPeffect;

#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Rendering/Enums/OPattributes.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPtextureCube.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Math/include/Vectors.h"
#include "./Math/include/OPmat4.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"

struct OPshaderUniformBufferUniform {
	OPchar* name = NULL;
	OPshaderUniformType::Enum type;
	ui32 size;
	ui32 offset;
	ui32 arrayStride;
	ui32 matrixStride;

	inline void Destroy() {
		if (name != NULL) {
			OPfree(name);
		}
	}
};

struct OPshaderUniformBuffer {
	void* internalPtr;

	OPchar* name = NULL;
	OPshaderUniformBufferUniform* uniforms = NULL;
	ui32 uniformCount = 0;
	ui32 size;
	void* data = NULL;

	inline void Bind() {
		OPRENDERER_ACTIVE->ShaderUniformBuffer.Bind(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->ShaderUniformBuffer.Destroy(this);
		if (name != NULL) {
			OPfree(name);
		}

		if (uniforms != NULL) {
			for (ui32 i = 0; i < uniformCount; i++) {
				uniforms[i].Destroy();
			}
			OPfree(uniforms);
		}

		if (data != NULL) {
			OPfree(data);
		}
	}
};

struct OPeffect {
	void* internalPtr;
	OPshader* vertexShader;
	OPshader* fragmentShader;
	OPshaderUniform* uniforms;
	OPshaderUniformBuffer* uniformBuffers;
	ui8 uniformCount = 0;
	ui8 uniformBufferCount = 0;

	OPeffect() {}
	OPeffect(OPshader* vert, OPshader* frag) { Init(vert, frag, NULL); }
	OPeffect(const OPchar* vert, const OPchar* frag) { Init(vert, frag, NULL); }
	OPeffect(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) { Init(vert, frag, vertexLayout); }
	OPeffect(const OPchar* vert, const OPchar* frag, OPvertexLayout* vertexLayout) { Init(vert, frag, vertexLayout); }

	void Init(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout);
	inline void Init(OPshader* vert, OPshader* frag) {
		Init(vert, frag, NULL);
	}
	inline void Init(const OPchar* vert, const OPchar* frag, OPvertexLayout* vertexLayout) {
		Init((OPshader*)OPCMAN.LoadGet(vert), (OPshader*)OPCMAN.LoadGet(frag), vertexLayout);
	}
	inline void Init(const OPchar* vert, const OPchar* frag) {
		Init(vert, frag, NULL);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->Effect.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->Effect.Unbind(this);
	}

	inline void Destroy() {

		for (ui32 i = 0; i < uniformCount; i++) {
			uniforms[i].Destroy();
		}
		OPfree(uniforms);


		for (ui32 i = 0; i < uniformBufferCount; i++) {
			uniformBuffers[i].Destroy();
		}
		OPfree(uniformBuffers);

		OPRENDERER_ACTIVE->Effect.Destroy(this);
	}

    inline void Free() {
        Destroy();
        OPfree(this);
    }

	inline bool AddUniform(const OPchar* name) { 
		return OPRENDERER_ACTIVE->Effect.AddUniform(this, name); 
	}

	OPshaderUniform* GetUniform(const OPchar* name);
	OPshaderUniformBuffer* GetUniformBuffer(const OPchar* name);
	OPshaderUniformBufferUniform* GetUniformBufferUniform(OPshaderUniformBuffer* ubo, const OPchar* name);

	inline void Set(OPshaderUniformBuffer* ubo, OPshaderUniformBufferUniform* shaderUniform, void* data, ui32 loc) {
		OPRENDERER_ACTIVE->ShaderUniformBuffer.Set(ubo, shaderUniform, data, loc);
	}

	inline void Set(OPshaderUniform* shaderUniform, void* data, ui32 loc) {
		OPRENDERER_ACTIVE->ShaderUniform.Set(shaderUniform, data, loc); 
	}


	inline void Set(OPshaderUniformBuffer* ubo, const OPchar* shaderUniformName, void* data) {
		OPshaderUniformBufferUniform* shaderUniform = GetUniformBufferUniform(ubo, shaderUniformName);
		OPRENDERER_ACTIVE->ShaderUniformBuffer.Set(ubo, shaderUniform, data, 0);
	}

	inline void Set(const OPchar* uboName, const OPchar* shaderUniformName, void* data) {
		OPshaderUniformBuffer* ubo = GetUniformBuffer(uboName);
		Set(ubo, shaderUniformName, data);
	}



	inline void Set(OPshaderUniform* shaderUniform, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetBool(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexture(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(shaderUniform, count, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(shaderUniform, count, val, slot); }

	inline void Set(const OPchar* name, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(GetUniform(name), val); }
	inline void Set(const OPchar* name, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPtexture* val, ui32 slot) {
		OPshaderUniform* uniform = GetUniform(name);
		if (uniform == NULL) {
			//OPlogErr("Shader Uniform %s not found", name);
			return;
		}
		OPRENDERER_ACTIVE->ShaderUniform.SetTexture(uniform, val, slot);
	}
	inline void Set(const OPchar* name, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(GetUniform(name), count, val, slot); }
	inline void Set(const OPchar* name, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(GetUniform(name), val, slot); }
	inline void Set(const OPchar* name, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(GetUniform(name), count, val, slot); }

	void UpdateVertexLayout(OPvertexLayout* vertexLayout);

	inline static OPeffect* Create(OPshader* vert, OPshader* frag) {
		OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		effect->Init(vert, frag);
		return effect;
	}
};


inline void OPeffectSet(const OPchar* name, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetBool(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexture(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val, slot); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val, slot); }
inline void OPeffectSet(const OPchar* name, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val, slot); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val, slot); }
inline void OPeffectSet(OPcam* val) {
	OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uView"), &val->view);
	OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uProj"), &val->proj);
}
