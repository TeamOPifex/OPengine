#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 vUV;

void main()
{
    vUV = aUV;
	gl_Position = vec4(aPos, 1.0);
}