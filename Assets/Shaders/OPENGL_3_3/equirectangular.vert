#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aN;

out vec3 vWorldPos;
out vec3 vN;

uniform mat4 uProj;
uniform mat4 uView;

void main()
{
vN = aN;
    vWorldPos = aPos;  
    gl_Position =  uProj * uView * vec4(vWorldPos, 1.0);
}