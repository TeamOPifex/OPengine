#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBinormal;
layout (location = 4) in vec2 aUV;

out vec3 vWorldPos;
out vec3 vFragPos;
out vec2 vUV;
out vec3 vNormal;
out vec4 vFragPosLightSpace;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uViewShadow;
uniform mat4 uProjShadow;

void main()
{
    vec4 viewPos = uView * uWorld * vec4(aPosition, 1.0f);
    vec4 worldPos = uWorld * vec4(aPosition, 1.0f);
	vWorldPos = worldPos.xyz;
    vFragPos = viewPos.xyz;
    gl_Position = uProj * viewPos;
    vUV = aUV;
    
    mat3 normalMatrix = transpose(inverse(mat3(uView * uWorld)));
    vNormal = normalMatrix * aNormal;

    vFragPosLightSpace = uProjShadow * uViewShadow * worldPos;
}