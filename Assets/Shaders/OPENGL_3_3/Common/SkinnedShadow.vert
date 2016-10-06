#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;
layout(location = 5) in vec4 aBlendIndices;
layout(location = 6) in vec4 aBlendWeights;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

// Values that stay constant for the whole mesh.
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uViewShadow;
uniform mat4 uProjShadow;
uniform mat4 uBones[100];

void main(){

    // This int cast is inneficient, we'll get to fixing it
    // But it requires a little back end work to not just do
    // glEnableAtribPtr regardless of type

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

	//newPosition = vec4(aPosition, 1.0);

  	// Output position of the vertex, in clip space : uMVP * position
  	gl_Position = uProj * uView * uWorld * newPosition;

    vs_out.FragPos = vec3(uWorld * newPosition);
    vs_out.Normal = transpose(inverse(mat3(uWorld))) * aNormal;
    vs_out.TexCoords = vec2(aUV.x, 1.0 - aUV.y);
    vs_out.FragPosLightSpace = uProjShadow * uViewShadow * vec4(vs_out.FragPos, 1.0);
}
