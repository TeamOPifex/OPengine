#include "Data/Resources/Material/Shader/ShaderTypes.h"
#include <GL/glew.h>

namespace OPifex{
	namespace ShaderTypes{
		GLenum gl_shader_type(OPifex::ShaderTypes::ShaderType shaderType);
		OPifex::ShaderTypes::ShaderType to_shader_type(GLenum shaderType);
	}
}