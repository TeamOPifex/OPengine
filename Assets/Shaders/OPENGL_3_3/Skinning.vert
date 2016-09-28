#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;
layout(location = 5) in vec4 aBlendIndices;
layout(location = 6) in vec4 aBlendWeights;

uniform mat4 uBones[62];

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightPosition;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vEyeDirection;
out vec3 vLightDirection;

void main() {
	vec4 pos = vec4(aPosition, 1.0);
	vec4 norm = vec4(aNormal, 0.0);
	
	
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

	
	vec4 newPosition = BoneTransform * pos;
	vec4 newNormal = BoneTransform * norm;

	//newPosition = pos;
	
	vec4 worldPos = uWorld * newPosition;
	gl_Position = (uProj * uView) * worldPos; 
	
	vTexCoord = aUV; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( uView * uWorld * vec4(aPosition,1)).xyz;
	vEyeDirection = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( uView * vec4(uLightPosition,1)).xyz;
	vLightDirection = LightPosition_cameraspace + vEyeDirection;
	
	vNormal = ( uView * uWorld * newNormal).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	vTangent = normalize(rotScl * aTangent); 
}
