#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;

//layout (location = 0) in vec3 aPosition;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec3 aTangent;
//layout (location = 3) in vec3 aBinormal;
//layout (location = 4) in vec2 aUV;

out vec4 vPos;
out vec2 vUV;
out mat4 vInvViewProj;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 worldPos = uWorld * vec4(aPosition, 1.0f);
    gl_Position = uProj * uView * worldPos;

	mat4 viewProj = uProj * uView;
	vInvViewProj = inverse(viewProj);

	vPos = gl_Position;
}