#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vUV = aUV;//vec2(aUV.x, 1.0 - aUV.y); // 
    vWorldPos = vec3(uWorld * vec4(aPosition, 1.0));
    vNormal = mat3(uWorld) * aNormal;   

    gl_Position =  uProj * uView * vec4(vWorldPos, 1.0);
}
