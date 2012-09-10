#include <stdio.h>

#include "Human\Rendering\RenderSystem.h"
#include "Human\Rendering\GL\GLShader.h"
#include "Human\Rendering\GL\GLMaterial.h"
#include "Human\Rendering\GL\GLBuffer.h"

using namespace OPifex;

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}\n";

int main(){
	printf("Program Started.");

	RenderSystem::Initialize();
	
	ShaderPtr vertex = new GLShader(OPifex::ShaderTypes::Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(OPifex::ShaderTypes::Fragment, gFragmentShader);
	GLMaterial program;
	program.load(vertex, pixel);
	ui32 position = program.attribute_location("vPosition");

	//RenderSystem::SetViewport(0, 0, 640, 480);
	
	static const f32 g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLBuffer buff;
	buff.load(BufferType::Vertex, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);

	do{
		RenderSystem::ClearColor(1.0f, 0.0f, 0.0f);
		RenderSystem::UseMaterial(&program);
		program.enable_attrib(0);
		RenderSystem::SetBuffer(buff.handle());
		program.set_data(0, 3, false, 0, (void*)0);
		RenderSystem::RenderTriangles(0, 3);
		program.disable_attrib(0);
		RenderSystem::Present();
	}
	while(RenderSystem::escape());
	
	printf("Program Ended.");

	return 0;
}
