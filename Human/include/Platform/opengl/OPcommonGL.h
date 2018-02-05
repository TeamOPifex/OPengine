#pragma once

#include "./Core/include/OPtypes.h"
#include "./Human/include/Platform/OPrendererDefines.h"
#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"
#include "./Human/include/Rendering/Enums/OPtextureWrap.h"
#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Human/include/Rendering/Enums/OPtextureFormat.h"
#include "./Human/include/Rendering/Enums/OPtextureType.h"

#ifdef OPIFEX_OPENGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#ifndef OPIFEX_WINDOWS
	#define __debugbreak __builtin_trap
#endif

bool OPglewInit();
GLenum OPcommonGLCheckError();
bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line);

//OPlog("%s, %s, %d", #x, __FILE__, __LINE__);

#ifdef _DEBUG
#define OPGLFN(x) OPcommonGLCheckError();\
		x; \
		if (!OPcommonGLLog(#x, __FILE__, __LINE__)) { \
            __debugbreak(); \
        }
#else
	#define OPGLFN(x) x
#endif

OPshaderUniformType::Enum UniformTypeToOPshaderUniformType(GLint type);
ui32 OPtextureWrapToGL(OPtextureWrap::Enum textureWrap);
ui32 OPtextureFormatToGL(OPtextureFormat::Enum textureFormat);
ui32 OPtextureFilterToGL(OPtextureFilter::Enum textureFilter);
ui32 OPtextureTypeToGL(OPtextureType::Enum textureType);

#endif