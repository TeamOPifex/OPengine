#include <stdio.h>

#include "Human\Rendering\RenderSystem.h"
#include "Human\Rendering\GL\GLShader.h"
#include "Human\Rendering\GL\GLMaterial.h"
#include "Human\Rendering\GL\GLBuffer.h"
#include "Human\Math\Matrix4.h"

#include "Human\Resources\Texture\Texture.h"

static const char gVertexShader[] = 
	"#version 330 core\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
	"layout(location = 1) in vec2 vertexUV;"
	"out vec2 UV;\n"
	"uniform mat4 MVP;\n"
	"void main(){\n"
    "gl_Position = MVP * vec4(vertexPosition_modelspace,1);\n"
	"UV = vertexUV;\n"
	"}";

static const char gFragmentShader[] = 
	"#version 330 core \n"
	"in vec2 UV; \n"
	"out vec3 color; \n"
	"uniform sampler2D myTextureSampler; \n"
    "void main() {\n"
	"  color = texture2D( myTextureSampler, UV ).rgb \n"
    "}\n";

static const f32 gTriangleVertices[] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f };

static const f32 g_uv_buffer_data[] = { 
	0.000059f, 1.0f, 
	0.000103f, 1.0f-0.336048f, 
	0.335973f, 1.0f-0.335903f, 
};

int main(){
	printf("Program Started.");

	RenderSystem::Initialize(RendererType::OpenGL_3_3);

	Matrix4 v, p, m;
	m.SetIdentity();
	m.SetIdentity();
	p = Matrix4::CreatePerspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);
	v = Matrix4::CreateLook(Vector3(4,3,3), Vector3(0), Vector3(0,1,0));
	m = m * v * p;

	// Load up the Vertex and Fragment Shaders
	// Then create a material (OpenGL Program) with the shaders
	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);
	MaterialPtr material = new GLMaterial(vertex, pixel);
	
	ui32 mvpLoc = material->uniform_location("MVP");
	ui32 sampLoc = material->uniform_location("myTextureSampler");
	
	// Create a Vertex Buffer with the triangle data
	BufferPtr buffer = new GLBuffer(BufferType::VertexBuffer, sizeof(gTriangleVertices), gTriangleVertices);

	BufferPtr uv = new GLBuffer(BufferType::VertexBuffer, sizeof(g_uv_buffer_data), g_uv_buffer_data);

	TextureDDS* dds = new TextureDDS("C:\\Repos\\OPifex Entertainment\\OPifex.Hg\\Engine\\Application\\Debug\\uvtemplate.DDS");
	GLTexture tex = GLTexture(dds);
	delete(dds);

	//ui32 textureLoc = material->uniform_location("texSampler");
	

	do{
		// Clear the back buffer
		RenderSystem::ClearColor(0.0f, 0.0f, 1.0f);

		// Set the material data to use
		RenderSystem::UseMaterial(material);
		material->set_matrix(mvpLoc, &m[0][0]);
		
		material->enable_attrib(0);
		RenderSystem::SetBuffer(buffer->handle());
		material->set_data(mvpLoc, 3, false, 0, (void*)0);
		
		material->enable_attrib(1);
		RenderSystem::SetBuffer(uv->handle());
		material->set_data(1, 2, false, 0, (void*)0);

		// Draw the triangle
		RenderSystem::RenderTriangles(0, 3);

		// Clean up
		material->disable_attrib(0);
		material->disable_attrib(1);

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
