
#include "./Human/include/Platform/opengl/OPshaderUniformBufferAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"
#include "./Core/include/Assert.h"

GLuint sizeFromUniformType(GLint type)
{
	GLuint s;

#define UNI_CASE(type, numElementsInType, elementType) \
      case type : s = numElementsInType * sizeof(elementType); break;

	switch (type)
	{
		UNI_CASE(GL_FLOAT, 1, GLfloat);
		UNI_CASE(GL_FLOAT_VEC2, 2, GLfloat);
		UNI_CASE(GL_FLOAT_VEC3, 3, GLfloat);
		UNI_CASE(GL_FLOAT_VEC4, 4, GLfloat);
		UNI_CASE(GL_INT, 1, GLint);
		UNI_CASE(GL_INT_VEC2, 2, GLint);
		UNI_CASE(GL_INT_VEC3, 3, GLint);
		UNI_CASE(GL_INT_VEC4, 4, GLint);
		UNI_CASE(GL_UNSIGNED_INT, 1, GLuint);
		UNI_CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		UNI_CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		UNI_CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
		UNI_CASE(GL_BOOL, 1, GLboolean);
		UNI_CASE(GL_BOOL_VEC2, 2, GLboolean);
		UNI_CASE(GL_BOOL_VEC3, 3, GLboolean);
		UNI_CASE(GL_BOOL_VEC4, 4, GLboolean);
		UNI_CASE(GL_FLOAT_MAT2, 4, GLfloat);
		UNI_CASE(GL_FLOAT_MAT3, 9, GLfloat);
		UNI_CASE(GL_FLOAT_MAT4, 16, GLfloat);
		UNI_CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		UNI_CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		UNI_CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		UNI_CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		UNI_CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		UNI_CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
	default: s = 0; break;
	}
	return s;
}

const OPchar* UniformTypeToString(GLint type)
{
	const OPchar* s;
	switch (type)
	{
	case GL_FLOAT: s = "GL_FLOAT"; break;
	case GL_FLOAT_VEC2: s = "GL_FLOAT_VEC2"; break;
	case GL_FLOAT_VEC3: s = "GL_FLOAT_VEC3"; break;
	case GL_FLOAT_VEC4: s = "GL_FLOAT_VEC4"; break;
	case GL_INT: s = "GL_INT"; break;
	case GL_INT_VEC2: s = "GL_INT_VEC2"; break;
	case GL_INT_VEC3: s = "GL_INT_VEC3"; break;
	case GL_INT_VEC4: s = "GL_INT_VEC4"; break;
	case GL_UNSIGNED_INT: s = "GL_UNSIGNED_INT"; break;
	case GL_UNSIGNED_INT_VEC2: s = "GL_UNSIGNED_INT_VEC2"; break;
	case GL_UNSIGNED_INT_VEC3: s = "GL_UNSIGNED_INT_VEC3"; break;
	case GL_UNSIGNED_INT_VEC4: s = "GL_UNSIGNED_INT_VEC4"; break;
	case GL_BOOL: s = "GL_BOOL"; break;
	case GL_BOOL_VEC2: s = "GL_BOOL_VEC2"; break;
	case GL_BOOL_VEC3: s = "GL_BOOL_VEC3"; break;
	case GL_BOOL_VEC4: s = "GL_BOOL_VEC4"; break;
	case GL_FLOAT_MAT2: s = "GL_FLOAT_MAT2"; break;
	case GL_FLOAT_MAT3: s = "GL_FLOAT_MAT3"; break;
	case GL_FLOAT_MAT4: s = "GL_FLOAT_MAT4"; break;
	case GL_FLOAT_MAT2x3: s = "GL_FLOAT_MAT2x3"; break;
	case GL_FLOAT_MAT2x4: s = "GL_FLOAT_MAT2x4"; break;
	case GL_FLOAT_MAT3x2: s = "GL_FLOAT_MAT3x2"; break;
	case GL_FLOAT_MAT3x4: s = "GL_FLOAT_MAT3x4"; break;
	case GL_FLOAT_MAT4x2: s = "GL_FLOAT_MAT4x2"; break;
	case GL_FLOAT_MAT4x3: s = "GL_FLOAT_MAT4x3"; break;
	default: s = "Unknown"; break;
	}
	return s;
}

ui32 BINDING_POINT = 0;

OPshaderUniformBuffer* OPshaderUniformBufferGLInit(OPshaderUniformBuffer* shaderUniformBuffer, OPeffect* effect, OPint index) {
	OPshaderUniformBufferGL* shaderUniformBufferGL = (OPshaderUniformBufferGL*)OPalloc(sizeof(OPshaderUniformBufferGL));
	shaderUniformBuffer->internalPtr = shaderUniformBufferGL;

	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;


	// get size of name of the uniform block
	GLint nameLength;
	glGetActiveUniformBlockiv(effectGL->Handle, index, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);

	// get name of uniform block
	OPchar* blockName = OPALLOC(OPchar, nameLength);
	glGetActiveUniformBlockName(effectGL->Handle, index, nameLength, nullptr, blockName);

	OPlogInfo("Uniform Block: %s", blockName);



	// get size of uniform block in bytes
	GLint uboSize;
	glGetActiveUniformBlockiv(effectGL->Handle, index, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
	OPlogInfo("Uniform Block Size: %d", uboSize);

	// get number of uniform variables in uniform block
	GLint numberOfUniformsInBlock;
	glGetActiveUniformBlockiv(effectGL->Handle, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numberOfUniformsInBlock);
	OPlogInfo("Uniforms: %d", numberOfUniformsInBlock);
	
	GLint* uniformsIndices = OPALLOC(GLint, numberOfUniformsInBlock);
	glGetActiveUniformBlockiv(effectGL->Handle, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformsIndices);

	shaderUniformBuffer->name = blockName;
	shaderUniformBuffer->size = uboSize;
	shaderUniformBuffer->data = OPalloc(shaderUniformBuffer->size);
	shaderUniformBuffer->uniforms = OPALLOC(OPshaderUniformBufferUniform, numberOfUniformsInBlock);
	shaderUniformBuffer->uniformCount = 0;
	

	// create Uniform Buffer Object and copy data
	glGenBuffers(1, &shaderUniformBufferGL->Handle);
	glBindBuffer(GL_UNIFORM_BUFFER, shaderUniformBufferGL->Handle);
	glBufferData(GL_UNIFORM_BUFFER, shaderUniformBuffer->size, NULL, GL_STREAM_DRAW);
	// glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// copy UBO and uniform block. Note: use binding points if you want to share this UBO among shaders
	// glBindBufferBase(GL_UNIFORM_BUFFER, effectGL->Handle, shaderUniformBufferGL->Handle);

	BINDING_POINT++;
	glUniformBlockBinding(effectGL->Handle, index, BINDING_POINT);
	glBindBufferRange(GL_UNIFORM_BUFFER, BINDING_POINT, shaderUniformBufferGL->Handle, 0, shaderUniformBuffer->size);

	// get parameters of all uniform variables in uniform block
	for (ui32 uniformMember = 0; uniformMember < numberOfUniformsInBlock; uniformMember++)
	{
		ui32 tUniformIndex = uniformsIndices[uniformMember];
		if (tUniformIndex >= 0)
		{
			GLint uniformNameLength, uniformOffset, uniformSize;
			GLint uniformType, arrayStride, matrixStride;

			GLsizei gllength;
			GLint glsize;
			GLenum gltype;


			// get length of name of uniform variable
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_NAME_LENGTH, &uniformNameLength);
			// get name of uniform variable
			OPchar* uniformName = OPNEW(OPchar[uniformNameLength]);
			glGetActiveUniform(effectGL->Handle, tUniformIndex, uniformNameLength, &gllength, &glsize, &gltype, uniformName);


			// get offset of uniform variable related to start of uniform block
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);
			// get size of uniform variable (number of elements)
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_SIZE, &uniformSize);
			// get type of uniform variable (size depends on this value)
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_TYPE, &uniformType);
			// offset between two elements of the array
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);
			// offset between two vectors in matrix
			glGetActiveUniformsiv(effectGL->Handle, 1, &tUniformIndex, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);

			// Size of uniform variable in bytes
			GLuint sizeInBytes = uniformSize * sizeFromUniformType(uniformType);

			// output data
			OPlogInfo("   Uniform[%d]: %s", tUniformIndex, uniformName);
			OPlogInfo("      Size: %d", sizeInBytes);
			OPlogInfo("      Offset: %d", uniformOffset);
			OPlogInfo("      Type: %d", UniformTypeToString(uniformType));
			OPlogInfo("      Array Stride: %d", arrayStride);
			OPlogInfo("      Matrix Stride: %d", matrixStride);

			//OPshaderUniformBufferUniform* uniform = OPNEW(OPshaderUniformBufferUniform);
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].name = uniformName;
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].size = sizeInBytes;
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].offset = uniformOffset;
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].type = UniformTypeToOPshaderUniformType(uniformType);
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].arrayStride = arrayStride;
			shaderUniformBuffer->uniforms[shaderUniformBuffer->uniformCount].matrixStride = matrixStride;
			shaderUniformBuffer->uniformCount++;

			// = .Put(uniformName, uniform);

		}
		else {
			OPlogErr("Bad Uniform");
		}

	}

	OPfree(uniformsIndices);


	return shaderUniformBuffer;
}

OPshaderUniformBuffer* OPshaderUniformBufferGLCreate(OPeffect* effect, OPint index) {
	OPshaderUniformBuffer* shaderUniformBuffer = (OPshaderUniformBuffer*)OPalloc(sizeof(OPshaderUniformBuffer));
	return OPshaderUniformBufferGLInit(shaderUniformBuffer, effect, index);
}

void OPshaderUniformBufferGLSet(OPshaderUniformBuffer* shaderUniformBuffer, OPshaderUniformBufferUniform* uniformBufferUniform, void* data, ui32 loc) {
	OPshaderUniformBufferGL* shaderUniformBufferGL = (OPshaderUniformBufferGL*)shaderUniformBuffer->internalPtr;
	void* dest = (void*)((OPint)shaderUniformBuffer->data + uniformBufferUniform->offset);
	OPmemcpy(dest, data, uniformBufferUniform->size);
}

void OPshaderUniformBufferGLBind(OPshaderUniformBuffer* shaderUniformBuffer) {
	OPshaderUniformBufferGL* shaderUniformBufferGL = (OPshaderUniformBufferGL*)shaderUniformBuffer->internalPtr;
	
	glBindBuffer(GL_UNIFORM_BUFFER, shaderUniformBufferGL->Handle);
	glBufferData(GL_UNIFORM_BUFFER, shaderUniformBuffer->size, shaderUniformBuffer->data, GL_STATIC_DRAW);

}

void OPshaderUniformBufferGLDestroy(OPshaderUniformBuffer* shaderUniformBuffer) {
	OPshaderUniformBufferGL* shaderUniformBufferGL = (OPshaderUniformBufferGL*)shaderUniformBuffer->internalPtr;

	OPfree(shaderUniformBufferGL);
	shaderUniformBuffer->internalPtr = NULL;
}


void OPshaderUniformBufferAPIGLInit(OPshaderUniformBufferAPI* shaderUniformBuffer) {
	shaderUniformBuffer->Create = OPshaderUniformBufferGLCreate;
	shaderUniformBuffer->Init = OPshaderUniformBufferGLInit;
	shaderUniformBuffer->Set = OPshaderUniformBufferGLSet;
	shaderUniformBuffer->Bind = OPshaderUniformBufferGLBind;
	shaderUniformBuffer->Destroy = OPshaderUniformBufferGLDestroy;
}

#endif