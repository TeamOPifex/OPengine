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

	OPLog("OPshader BEGINNING load of %s", filename);
	OPstream* source = OPreadFile(filename);
	OPLog("OPshader ENDING load of %s", filename);
	
	CheckError("GLShader::Error 0");
	OPLog("OPshader shader CREATING");
	vertex = glCreateShader(OPvertexShader);
	OPLog("OPshader shader CREATED");
	CheckError("GLShader::Error 1");
	if(vertex){
		OPchar* src = (OPchar*)source->Data;

		OPLog("[%s]\n", src);

		OPLog("OPrenderLoadVertexShader::Finished Printing Source");
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

	CheckError("GLShader::Error 6");

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
	CheckError("GLShader::Error 6");

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
OPeffect OPrenderCreateEffect(OPshader vert, OPshader frag, OPshaderAttribute* Attributes, OPint AttribCount){
	OPeffect effect = {
		vert,
		frag,
		-1,
		 0
	};
	CheckError("OPrenderCreateEffect:Error 1");
	effect.Parameters = OPhashMapCreate(2);
	effect.Attributes = OPlistCreate(AttribCount, sizeof(OPshaderAttribute));
	CheckError("OPrenderCreateEffect:Error 2");

	effect.ProgramHandle = glCreateProgram();
	CheckError("OPrenderCreateEffect:Error 3");

	glAttachShader(effect.ProgramHandle, vert);
	CheckError("OPrenderCreateEffect:Error 4");
	glAttachShader(effect.ProgramHandle, frag);
	CheckError("OPrenderCreateEffect:Error 5");
	glLinkProgram(effect.ProgramHandle);
	CheckError("OPrenderCreateEffect:Error 6");

	OPint status;
	glGetProgramiv(effect.ProgramHandle, GL_LINK_STATUS, &status);

	if(status == GL_FALSE) {
		OPLog("FAILED to link Shader Program");
	}
	
	OPLog("Shader Status: %d", status);
	CheckError("OPrenderCreateEffect:Error 7");

	// create, and copy attributes into list
	for(OPint i = 0; i < AttribCount; i++){
		OPshaderAttribute attr = {
			NULL,
			Attributes[i].Type,
			Attributes[i].Elements,
			(void*)effect.Stride
		};

		attr.Name = (OPchar*)glGetAttribLocation(
			effect.ProgramHandle,
			Attributes[i].Name
		);
		if(CheckError("OPrenderCreateEffect:Error 7.5 - Attrib Could not be found.") > 0) {
			OPLog("Handle: %d, Attribute: %s", effect.ProgramHandle, Attributes[i].Name);
		}

		// TODO add more
		switch(Attributes[i].Type){
			case GL_FLOAT:
				effect.Stride += (4 * Attributes[i].Elements);
				break;
		}

		//OPLog("%s = %d loc %d\n", Attributes[AttribCount], AttribCount, *loc);
		OPlistPush(effect.Attributes, (ui8*)&attr);
	}
	
	CheckError("OPrenderCreateEffect:Error 8");
	return effect;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect){
	return 1;
}
//-----------------------------------------------------------------------------
// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect){
	OPhashMapDestroy(effect->Parameters);
	OPlistDestroy(effect->Attributes);
	glDeleteProgram(effect->ProgramHandle);
	
	return 1;
}

// effect managment
OPint OPrenderBindEffect(OPeffect* effect){
	CheckError("OPrenderBindEffect:Error 0");
	// disable attributes of the last effect
	if(OPRENDER_CURR_EFFECT){
		OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
		for(;attrCount--;){
	CheckError("OPrenderBindEffect:Error 0.2");
			OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
	CheckError("OPrenderBindEffect:Error 0.3");
			glDisableVertexAttribArray((ui32)attr->Name);
			if(CheckError("OPrenderBindEffect:Error 0.4")) {
				//OPLog("Failed to bind %s", attr->Name);
			}
		}
	}
	CheckError("OPrenderBindEffect:Error 1");

	OPRENDER_CURR_EFFECT = effect;

	glUseProgram(OPRENDER_CURR_EFFECT->ProgramHandle);
	CheckError("OPrenderBindEffect:Error 2");
	// enable attributes of the new effect
	OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
	for(;attrCount--;){
		OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
		
	CheckError("OPrenderBindEffect:Error 3");
		glEnableVertexAttribArray((ui32)attr->Name);
	CheckError("OPrenderBindEffect:Error 4");
		glVertexAttribPointer(
			(ui32)attr->Name,
			attr->Elements,
			attr->Type,
			GL_FALSE,
			effect->Stride,
			attr->Offset
		);
	CheckError("OPrenderBindEffect:Error 5");
	}

	return 1;
}

ui32 OPrenderGetParam(const OPchar* parameterName){
	if(OPhashMapExists(OPRENDER_CURR_EFFECT->Parameters, parameterName)){
		ui32* loc;
		OPhashMapGet(
			OPRENDER_CURR_EFFECT->Parameters,
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
		OPhashMapPut(OPRENDER_CURR_EFFECT->Parameters, parameterName, &locPtr);
		return loc;
	}
}

// parameter setting
/*
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
*/
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
