#pragma once

struct OPshaderUniformAPI;
typedef struct OPshaderUniformAPI OPshaderUniformAPI;

#include "./Math/include/OPmat3.h"
#include "./Math/include/OPmat4.h"

struct OPshaderUniformBuffer;
struct OPshaderUniformBufferUniform;
struct OPeffect;
struct OPtexture;
struct OPtextureCube;

struct OPshaderUniformBufferAPI {
	OPshaderUniformBuffer*(*Create)(OPeffect* effect, OPint index) = 0;
	OPshaderUniformBuffer*(*Init)(OPshaderUniformBuffer* shaderUniformBuffer, OPeffect* effect, OPint index) = 0;
	void(*Set)(OPshaderUniformBuffer* shaderUniformBuffer, OPshaderUniformBufferUniform* shaderUniformBufferUniform, void* data, ui32 loc) = 0;
	void(*Bind)(OPshaderUniformBuffer* shaderUniformBuffer) = 0;
	void(*Destroy)(OPshaderUniformBuffer* shaderUniform) = 0;
};