#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Human/Resources/Material/Shader/ShaderTypes.h"

GLenum gl_shader_type(ShaderType shaderType);
ShaderType to_shader_type(GLenum shaderType);
	