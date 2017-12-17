#pragma once

struct OPshaderAPI;
typedef struct OPshaderAPI OPshaderAPI;

#include "./Human/include/Rendering/Enums/OPshaderType.h"
#include "./Data/include/OPstream.h"
struct OPshader;

struct OPshaderAPI {
	OPshader*(*_Create)(OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) = 0;
	OPshader*(*_Init)(OPshader* shader, OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) = 0;
	void(*Destroy)(OPshader* shader) = 0;
	
	inline OPshader* Create(OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
		return _Create(shaderType, source, sourceLen);
	}

	inline OPshader* Create(OPshaderType::Enum shaderType, OPstream* source) {
		return _Create(shaderType, (OPchar*)source->Data, source->Length);
	}

	inline OPshader* Init(OPshader* shader, OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
		return Init(shader, shaderType, source, sourceLen);
	}

	inline OPshader* Init(OPshader* shader, OPshaderType::Enum shaderType, OPstream* source) {
		return Init(shader, shaderType, (OPchar*)source->Data, source->Length);
	}
};