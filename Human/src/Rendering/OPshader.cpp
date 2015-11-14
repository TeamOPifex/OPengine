#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
// Shader creation
OPint OPshaderLoadVertex(OPstream* source, OPshader** shader){
	OPshader vertex = -1;

	//OPstream* source = OPreadFile(filename);

	OPglError("GLShader::Error 0");
	vertex = glCreateShader(OPvertexShader);
	OPglError("GLShader::Error 1");
	if (vertex){
		OPchar* src = (OPchar*)source->Data;

		glShaderSource(vertex, 1, (const OPchar**)&src, 0);
		OPglError("GLShader::Error 2");
		glCompileShader(vertex);
		OPglError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
		OPglError("GLShader::Error 4");

		if (!compiled){
			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(vertex, 4096, &length, msg);
			OPlog("GLShader::Failed to compile Vertex Shader::%s", msg);

			OPglError("GLShader::Error 5");
			glDeleteShader(vertex);
			//OPstreamDestroy(source); // clean up stream
			return -1;
		}
	}
	else{
		//OPstreamDestroy(source); // clean up stream
		return -1;
	}
	//OPstreamDestroy(source); // clean up stream

	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = vertex; // copy the shader handle

	OPglError("GLShader::Error 6");

	return 1;
}

//-----------------------------------------------------------------------------
OPint OPshaderLoadFragment(OPstream* source, OPshader** shader){
	OPshader frag = -1;
	//OPstream* source = OPreadFile(filename);

	frag = glCreateShader(OPfragmentShader);

	if (frag){
		OPchar* src = (OPchar*)source->Data;
		glShaderSource(frag, 1, (const OPchar**)&src, 0);
		glCompileShader(frag);

		GLint compiled = 0;
		glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
		if (!compiled){

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(frag, 4096, &length, msg);

			OPlog("GLShader::Failed to compile Fragment Shader::%s", msg);

			glDeleteShader(frag);
			//OPstreamDestroy(source); // clean up stream
			return -1;
		}
	}
	else{
		//OPstreamDestroy(source); // clean up stream
		return -1;
	}
	//OPstreamDestroy(source); // clean up stream

	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = frag; // copy the shader handle

	return 1;
}
//-----------------------------------------------------------------------------
// shader destruction
OPint OPshaderUnload(OPshader* shader){
	glDeleteShader(*shader); // free up the gl shader
	OPfree(shader); // free up the integer
	return 1;
}
