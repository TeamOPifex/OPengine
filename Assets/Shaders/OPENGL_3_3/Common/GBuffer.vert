#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBinormal;
layout (location = 4) in vec2 aUV;

out vec3 vFragPos;
out vec2 vUV;
out vec3 vNormal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 worldPos = uWorld * vec4(aPosition, 1.0f);
    vFragPos = worldPos.xyz; 
    gl_Position = uProj * uView * worldPos;
    vUV = vec2(aUV.x, 1.0 - aUV.y);
    
    mat3 normalMatrix = transpose(inverse(mat3(uWorld)));
    vNormal = normalMatrix * aNormal;
}