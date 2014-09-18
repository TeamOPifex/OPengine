attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec2 aUV;
attribute vec4 aBlendIndices;
attribute vec4 aBlendWeights;

uniform mat4 uBones[62];

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightPosition;

varying vec2 vTexCoord; 
varying vec3 vNormal; 
varying vec3 vTangent; 
varying vec3 vEyeDirection; 
varying vec3 vLightDirection; 

void main() {
	vec4 pos = vec4(aPosition, 1.0);
	vec4 norm = vec4(aNormal, 0.0);
	
	vec4 newPosition = pos;
	vec4 newNormal = norm;
	
	
	
	
	
	int index = int(aBlendIndices.x);
	newPosition = (uBones[index] * pos) * aBlendWeights.x;
	newNormal = (uBones[index] * norm) * aBlendWeights.x;

	index = int(aBlendIndices.y);
	newPosition = (uBones[index] * pos) * aBlendWeights.y + newPosition;
	newNormal = (uBones[index] * norm) * aBlendWeights.y + newNormal;
	
	
	index = int(aBlendIndices.z);
	newPosition = (uBones[index] * pos) * aBlendWeights.z + newPosition;
	newNormal = (uBones[index] * norm) * aBlendWeights.z + newNormal;
	
	
	index = int(aBlendIndices.w);
	newPosition = (uBones[index] * pos) * aBlendWeights.w + newPosition;
	newNormal = (uBones[index] * norm) * aBlendWeights.w + newNormal;

	
	
	
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
