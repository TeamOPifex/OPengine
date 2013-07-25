#include "./Human/include/Resources/Material/Material.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Core/include/DynamicMemory.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

Material::Material(ShaderPtr vertex, ShaderPtr fragment){ 
	hashMap = OPhashMapCreate(8);

	m_handle = glCreateProgram();
	CheckError("GLMaterial::Error 1");

	if(m_handle){
		glAttachShader(m_handle, (GLuint)vertex->Handle());
		CheckError("GLMaterial::Error 2");

		glAttachShader(m_handle, (GLuint)fragment->Handle());
		CheckError("GLMaterial::Error 3");

		glLinkProgram(m_handle);
		CheckError("GLMaterial::Error 4");
	}
}

Material::~Material(){
	OPhashMapDestroy(hashMap);
}

ui32 Material::GetAttribute(const char* attribute){
	return glGetAttribLocation(m_handle, attribute);
}

ui32 Material::GetUniform(const char* attribute){
	return glGetUniformLocation(m_handle, attribute);
}

void Material::LoadAttribute(const char* attribute){
	OPint* pos = (OPint*)OPalloc(sizeof(OPint));
	*pos = glGetAttribLocation(m_handle, attribute);
	OPhashMapPut(hashMap, (OPchar*)attribute, pos);
}

void Material::LoadUniform(const char* attribute){
	OPint* pos = (OPint*)OPalloc(sizeof(OPint));
	*pos = glGetUniformLocation(m_handle, attribute);
	OPhashMapPut(hashMap, (OPchar*)attribute, pos);
}

void Material::BindAttribute(i32 pos, char* variable){
	glBindAttribLocation(m_handle, pos, variable);
}

void Material::EnableAttribute(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void Material::DisableAttribute(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}
	
void Material::SetData(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData){
	glVertexAttribPointer((GLuint)attribute, size, GL_FLOAT, normalize, stride, arrayData);
}

void Material::SetFloat(ui32 attribute, f32 value){
	glUniform1f(attribute, value);
}

void Material::SetVector2(ui32 attribute, Vector2* value){
	glUniform1fv(attribute, 2, &(*value)[0]);
}

void Material::SetVector3(ui32 attribute, Vector3* value){
	glUniform1fv(attribute, 3, &(*value)[0]);
}

void Material::SetVector4(ui32 attribute, Vector4* value){
	glUniform1fv(attribute, 4, &(*value)[0]);
}

void Material::SetMatrix(ui32 attribute, Matrix4* value){
	glUniformMatrix4fv(attribute, 1, GL_FALSE, &(*value)[0][0]);
}

void Material::SetFloat(const OPchar* attribute, f32 value){
	OPint* val;
	if(OPhashMapGet(hashMap, attribute, (void**)&val)){
		glUniform1f(*val, value);
	}
}

void Material::SetVector2(const OPchar* attribute, Vector2* value){	
	OPint* val;
	if(OPhashMapGet(hashMap, attribute, (void**)&val)){
		glUniform1fv(*val, 2, &(*value)[0]);
	}
}

void Material::SetVector3(const OPchar* attribute, Vector3* value){
	OPint* val;
	if(OPhashMapGet(hashMap, attribute, (void**)&val)){
		glUniform1fv(*val, 3, &(*value)[0]);
	}
}

void Material::SetVector4(const OPchar* attribute, Vector4* value){
	OPint* val;
	if(OPhashMapGet(hashMap, attribute, (void**)&val)){
		glUniform1fv(*val, 4, &(*value)[0]);
	}
}

void Material::SetMatrix(const OPchar* attribute, Matrix4* value){
	OPint* val;
	if(OPhashMapGet(hashMap, attribute, (void**)&val)){
		glUniformMatrix4fv(*val, 1, GL_FALSE, &(*value)[0][0]);
	}
}
