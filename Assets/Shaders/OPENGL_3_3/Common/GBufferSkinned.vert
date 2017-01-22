#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBinormal;
layout (location = 4) in vec2 aUV;
layout (location = 5) in vec4 aBlendIndices;
layout (location = 6) in vec4 aBlendWeights;

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
uniform mat4 uBones[100];

void main()
{
    int boneId0 = int(aBlendIndices.x);
    int boneId1 = int(aBlendIndices.y);
    int boneId2 = int(aBlendIndices.z);
    int boneId3 = int(aBlendIndices.w);

    mat4 BoneTransform = uBones[boneId0] * aBlendWeights.x;
    BoneTransform += uBones[boneId1] * aBlendWeights.y;
    BoneTransform += uBones[boneId2] * aBlendWeights.z;
    BoneTransform += uBones[boneId3] * aBlendWeights.w;

    vec4 newPosition = BoneTransform * vec4(aPosition, 1.0);
    vec4 newNormal = BoneTransform * vec4(aNormal, 0.0);


    vec4 viewPos = uView * uWorld * vec4(newPosition.xyz, 1.0f);
    vec4 worldPos = uWorld * vec4(newPosition.xyz, 1.0f);
	vWorldPos = worldPos.xyz;
    vFragPos = viewPos.xyz; 
    gl_Position = uProj * viewPos;
    vUV = aUV;
    
    mat3 normalMatrix = transpose(inverse(mat3(uView * uWorld)));
    vNormal = normalMatrix * newNormal.xyz;

    vFragPosLightSpace = uProjShadow * uViewShadow * worldPos;
}