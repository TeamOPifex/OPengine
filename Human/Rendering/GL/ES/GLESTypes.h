#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Data/Resources/Material/Shader/ShaderTypes.h"

namespace OPifex{
	namespace ShaderTypes{
		GLenum gl_shader_type(OPifex::ShaderTypes::ShaderType shaderType);
		OPifex::ShaderTypes::ShaderType to_shader_type(GLenum shaderType);
	}
}