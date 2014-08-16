attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aUV;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightPosition;

varying vec3 vNormal; 
varying vec2 vUV; 
varying vec3 vLightDirection; 

void main() {	
	
	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( uView * uWorld * vec4(aPosition,1)).xyz;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( uView * vec4(uLightPosition,1)).xyz;
	vLightDirection = LightPosition_cameraspace + (vec3(0,0,0) - vertexPosition_cameraspace);
	
	vNormal = (uView * uWorld * vec4(aNormal, 1.0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
