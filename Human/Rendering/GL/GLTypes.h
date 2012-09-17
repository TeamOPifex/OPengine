#include "Data/Resources/Material/Shader/ShaderTypes.h"
#include <GL/glew.h>

namespace ShaderTypes{
	GLenum gl_shader_type(ShaderTypes::ShaderType shaderType);
	ShaderTypes::ShaderType to_shader_type(GLenum shaderType);
}