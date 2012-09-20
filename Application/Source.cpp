#include <stdio.h>

#include "Human\Rendering\RenderSystem.h"
#include "Human\Rendering\GL\GLShader.h"
#include "Human\Rendering\GL\GLMaterial.h"
#include "Human\Rendering\GL\GLBuffer.h"

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";

	static const f32 g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

static const f32 gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

int main(){
	printf("Program Started.");

	RenderSystem::Initialize(RendererType::OpenGL_3_3);
	
	// Load up the Vertex and Fragent Shaders
	// Then create a material (OpenGL Program) with the shaders
	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);
	MaterialPtr material = new GLMaterial(vertex, pixel);

	// Create a Vertex Buffer with the triangle data
	BufferPtr buffer = new GLBuffer(BufferType::VertexBuffer, sizeof(g_vertex_buffer_data), gTriangleVertices);

	do{
		// Clear the back buffer
		RenderSystem::ClearColor(0.0f, 0.0f, 1.0f);

		// Set the material data to use
		RenderSystem::UseMaterial(material);
		material->enable_attrib(0);
		RenderSystem::SetBuffer(buffer->handle());
		material->set_data(0, 2, false, 0, (void*)0);

		// Draw the triangle
		RenderSystem::RenderTriangles(0, 3);

		// Clean up
		material->disable_attrib(0);

		// Swap the back buffer
		RenderSystem::Present();
	}
	while(RenderSystem::escape());
	
	printf("Program Ended.");
	
	delete buffer;
	delete material;
	delete vertex;
	delete pixel;

	return 0;
}
