#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;
layout(location = 5) in vec4 aBlendIndices;
layout(location = 6) in vec4 aBlendWeights;

// Values that stay constant for the whole mesh.
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uBones[62];

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

	   gl_Position =  uProj * uView * uWorld * newPosition;
}
