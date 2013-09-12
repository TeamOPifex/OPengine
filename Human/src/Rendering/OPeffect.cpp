#include "./Human/include/Rendering/OPeffect.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#endif


OPeffect* OPRENDER_CURR_EFFECT = NULL;
//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
// Shader creation
OPint OPrenderLoadVertexShader(const OPchar* filename, OPshader** shader){
	OPshader vertex = -1;
	OPstream* source = OPreadFile(filename);

	vertex = glCreateShader(OPvertexShader);
	CheckError("GLShader::Error 1");
	if(vertex){
		OPchar* src = (OPchar*)source->Data;
		OPLog("%s\n", src);
		glShaderSource(vertex, 1, (const OPchar**)&src, 0);
		CheckError("GLShader::Error 2");
		glCompileShader(vertex);
		CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
		CheckError("GLShader::Error 4");
		if(!compiled){
			OPLog("GLShader::Failed to compile Shader");

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(vertex, 4096, &length, msg);
			OPLog(msg);

			CheckError("GLShader::Error 5");
			glDeleteShader(vertex);
			OPstreamDestroy(source); // clean up stream
			return 0;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return 0;
	}
	OPstreamDestroy(source); // clean up stream
	
	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = vertex; // copy the shader handle

	return 1;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadFragmentShader(const OPchar* filename, OPshader** shader){
	OPshader frag = -1;
	OPstream* source = OPreadFile(filename);

	frag = glCreateShader(OPfragmentShader);
	CheckError("GLShader::Error 1");
	if(frag){
		OPchar* src = (OPchar*)source->Data;
		OPLog("%s\n", src);
		glShaderSource(frag, 1, (const OPchar**)&src, 0);
		CheckError("GLShader::Error 2");
		glCompileShader(frag);
		CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
		CheckError("GLShader::Error 4");
		if(!compiled){
			OPLog("GLShader::Failed to compile Shader");

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(frag, 4096, &length, msg);
			OPLog(msg);

			CheckError("GLShader::Error 5");
			glDeleteShader(frag);
			OPstreamDestroy(source); // clean up stream
			return 0;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return 0;
	}
	OPstreamDestroy(source); // clean up stream
	
	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = frag; // copy the shader handle

	return 1;
}
//-----------------------------------------------------------------------------
// shader destruction
OPint OPrenderUnloadShader(OPshader* shader){
	glDeleteShader(*shader); // free up the gl shader
	OPfree(shader); // free up the integer
	return 1;
}
//-----------------------------------------------------------------------------
// effect creation
OPeffect OPrenderCreateEffect(OPshader vert, OPshader frag, OPchar** Attributes, OPint AttribCount){
	OPeffect effect = {
		vert,
		frag,
		-1,
		{0},
		{0}
	};

	OPmemcpy(&effect.Parameters, OPhashMapCreate(2), sizeof(HashMap));
	OPmemcpy(&effect.Attributes, OPlistCreate(AttribCount, sizeof(ui32)), sizeof(OPlist));

	effect.ProgramHandle = glCreateProgram();

	glAttachShader(effect.ProgramHandle, vert);
	glAttachShader(effect.ProgramHandle, frag);

	glLinkProgram(effect.ProgramHandle);
	CheckError("Messed up");

	OPint status;
	glGetProgramiv(effect.ProgramHandle, GL_LINK_STATUS, &status);
	OPLog("True: %d Link status: %d, vert %d, frag %d\n", GL_TRUE, status, vert, frag);

	// create, and copy attributes into list
	for(;AttribCount--;){
		ui32* loc = (ui32*)OPalloc(sizeof(ui32));

		*loc = glGetAttribLocation(
			effect.ProgramHandle,
			Attributes[AttribCount]
		);

		OPLog("%s = %d loc %d\n", Attributes[AttribCount], AttribCount, *loc);
		OPlistPush(&effect.Attributes, (ui8*)loc);
	}

	return effect;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect){
	return 1;
}
//-----------------------------------------------------------------------------
// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect){
	return 1;
}

// effect managment
OPint OPrenderBindEffect(OPeffect* effect){
	// disable attributes of the last effect
	if(OPRENDER_CURR_EFFECT){
		OPint attrCount = OPlistSize(&OPRENDER_CURR_EFFECT->Attributes);
		for(;attrCount--;){
			ui32 loc = *((ui32*)OPlistGet(&OPRENDER_CURR_EFFECT->Attributes, attrCount));
			glDisableVertexAttribArray(loc);
		}
	}

	OPRENDER_CURR_EFFECT = effect;
	OPLog("ProgramHandle: %d\n", OPRENDER_CURR_EFFECT->ProgramHandle);
	glUseProgram(OPRENDER_CURR_EFFECT->ProgramHandle);
	// enable attributes of the new effect
	OPint attrCount = OPlistSize(&OPRENDER_CURR_EFFECT->Attributes);
	for(;attrCount--;){
		ui32 loc = *((ui32*)OPlistGet(&OPRENDER_CURR_EFFECT->Attributes, attrCount));
		OPLog("Attr %d -> loc %u\n", attrCount, loc);
		glEnableVertexAttribArray(loc);
	}

	return 1;
}

ui32 OPrenderGetParam(const OPchar* parameterName){
	if(OPhashMapExists(&OPRENDER_CURR_EFFECT->Parameters, parameterName)){
		ui32* loc;
		OPhashMapGet(
			&OPRENDER_CURR_EFFECT->Parameters,
			parameterName,
			(void**)&loc
		);
		return *loc;
	}
	else{
		ui32 loc = glGetUniformLocation(
			OPRENDER_CURR_EFFECT->ProgramHandle,
			parameterName
		);

		ui32* locPtr = (ui32*)OPalloc(sizeof(ui32));
		*locPtr = loc;
		OPhashMapPut(&OPRENDER_CURR_EFFECT->Parameters, parameterName, &locPtr);
		return loc;
	}
}

// parameter setting
void OPrenderParamf(ui32 param, OPfloat f){
	glUniform1f(param, f);
}
void OPrenderParamfv(ui32 param, OPint count, OPfloat* f){
	glUniform1fv(param, count, f);
}
void OPrenderParami(ui32 param, OPint i){
	glUniform1i(param, i);
}
void OPrenderParamiv(ui32 param, OPint count, OPint* i){
	glUniform1iv(param, count, i);
}
void OPrenderParamMat4v(ui32 param, OPint count, OPmat4* matrices){
	glUniformMatrix4fv(param, count, GL_FALSE, (OPfloat*)matrices);
}
void OPrenderUseTexture(ui32 param, ui32 texture, ui32 slot){
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(param, slot);
}
//-----------------------------------------------------------------------------
void OPrenderParamf(const OPchar* param, OPfloat f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1f(loc, f);
}
void OPrenderParamfv(const OPchar* param, OPint count, OPfloat* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1fv(loc, count, f);
}
void OPrenderParami(const OPchar* param, OPint i){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1i(loc, i);
}
void OPrenderParamiv(const OPchar* param, OPint count, OPint* i){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1iv(loc, count, i);
}
void OPrenderParamMat4v(const OPchar* param, OPint count, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniformMatrix4fv(loc, count, GL_FALSE, (OPfloat*)matrices);
}
void OPrenderUseTexture(const OPchar* param, ui32 texture, ui32 slot){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(loc, slot);
}
