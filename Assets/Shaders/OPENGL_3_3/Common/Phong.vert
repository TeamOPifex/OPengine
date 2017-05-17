#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;

// Declare an interface block; see 'Advanced GLSL' for what these are.
out vec3 vFragPos;
out vec3 vNormal;
out vec2 vUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    gl_Position = uProj * uView * uWorld * vec4(aPosition, 1.0f);
    vFragPos = aPosition;
    vNormal = aNormal;
    vUV = aUV;
}