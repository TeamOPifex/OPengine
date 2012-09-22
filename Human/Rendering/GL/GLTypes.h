#include "Data/Resources/Material/Shader/ShaderTypes.h"
#include <GL/glew.h>

GLenum gl_shader_type(ShaderType shaderType);
ShaderType to_shader_type(GLenum shaderType);
