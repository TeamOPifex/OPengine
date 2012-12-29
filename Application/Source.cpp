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
    "void main() { \n"
	"  color = texture2D( myTextureSampler, UV ).rgb; \n"
    "} \n";

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const f32 gTriangleVertices[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// Two UV coordinatesfor each vertex. They were created withe Blender.
	static const f32 g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};
int main(){
	printf("Program Started.");

	RenderSystem::Initialize(RendererType::OpenGL_3_3);

	//GLMaterial* arr = new GLMaterial[10];
	GLMaterial arr[10];


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

	TextureDDS* dds = new TextureDDS("E:\\bricks.DDS");
	GLTexture tex = GLTexture(dds);
	delete(dds);

	do{
	//	// Clear the back buffer
		RenderSystem::ClearColor(0.0f, 0.0f, 1.0f);

		// Set the material data to use
		RenderSystem::UseMaterial(material);
		
		material->set_matrix(mvpLoc, &m[0][0]);

		tex.bind(sampLoc);

		material->enable_attrib(0);
		RenderSystem::SetBuffer(buffer->handle());
		material->set_data(0, 3, false, 0, (void*)0);
		
		material->enable_attrib(1);
		RenderSystem::SetBuffer(uv->handle());
		material->set_data(1, 2, false, 0, (void*)0);
		
		// Draw the triangle
		RenderSystem::RenderTriangles(0, 12*3);

		// Clean up
		material->disable_attrib(0);
		material->disable_attrib(1);

	//	// Swap the back buffer
		RenderSystem::Present();
	}
	while(RenderSystem::escape());
	
	printf("Program Ended.");
	
	//delete buffer;
	//delete material;
	//delete vertex;
	//delete pixel;

	return 0;
}
