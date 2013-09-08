#include "./Human/include/Rendering/OPeffect.h"

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
		glShaderSource(vertex, 1, &source->Data, 0);
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
	*shader = (OPShader*)OPalloc(sizeof(OPshader));
	**shader = vertex; // copy the shader handle

	return 1;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadFragmentShader(const OPchar* filename, OPshader** shader){
	OPshader frag = -1;
	OPstream* source = OPreadFile(filename);

	frag = glCreateShader(OPvertexShader);
	CheckError("GLShader::Error 1");
	if(frag){
		glShaderSource(frag, 1, &source->Data, 0);
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
	*shader = (OPShader*)OPalloc(sizeof(OPshader));
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
		*OPhashMapCreate(2),
		*OPlistCreate(AttribCount, sizeof(ui32))
	};

	effect.ProgramHandle = glCreateProgram();

	// create, and copy attributes into list
	for(;AttribCount--;){
		ui32 loc = glGetAttribLocation(
			effect.ProgramHandle,
			Attributes[AttribCount]
		);
		OPlistPush(&effect.Attributes, &loc);
	}
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
		OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
		for(;attrCount--;){
			ui32 loc = *((ui32*)OPlistGet(&OPRENDER_CURR_EFFECT->Attributes, attrCount));
			glDisableVertexAttribArray(loc);
		}
	}

	OPRENDER_CURR_EFFECT = effect;
	glUseProgram(OPRENDER_CURR_EFFECT->ProgramHandle);
	// enable attributes of the new effect
	OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
	for(;attrCount--;){
		ui32 loc = *((ui32*)OPlistGet(&OPRENDER_CURR_EFFECT->Attributes, attrCount));
		glEnableVertexAttribArray(loc);
	}
}

ui32 OPrenderGetParam(const OPchar* parameterName){
	if(OPhashMapExists(&OPRENDER_CURR_EFFECT->Parameters, parameterName)){
		ui32* loc;
		OPhashMapGet(
			&OPRENDER_CURR_EFFECT->Parameters,
			parameterName,
			&loc
		);
		return *loc;
	}
	else{
		ui32 loc = glGetUniformLocation(
			OPRENDER_CURR_EFFECT->ProgramHandle,
			parameterName
		);

		ui32* locPtr = OPalloc(sizeof(ui32));
		*locPtr = loc;
		OPhasMapPut(&OPRENDER_CURR_EFFECT->Parameters, locPtr);
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
	glUniformMatrix4fv(param, count, GL_FALSE, matrices);
}