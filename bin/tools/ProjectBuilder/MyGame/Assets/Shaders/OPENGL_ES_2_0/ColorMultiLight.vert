attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aColor;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightPos; 
uniform vec3 uAmbient; 
uniform float uLightPower;

varying vec3 vColor; 
varying vec3 vNormal; 
varying vec3 vAmbient; 
varying vec3 vLightDirection; 
varying vec3 vEyePosition;
varying vec3 vLightPos; 
varying float vLightPower;
varying vec3 vWorldSpace; 

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	vWorldSpace = worldPos.xyz;
	gl_Position = uProj * uView * worldPos;
	
	vNormal = ( uView * uWorld * vec4(aNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	vec3 cameraPosition = ( uView * uWorld * vec4(aPosition,1)).xyz;
	vEyePosition = vec3(0,0,0) - cameraPosition;

	vColor = aColor;	
	vec3 lightPosition = (uView * vec4(uLightPos, 1)).xyz;
	vLightDirection = uLightPos;
	vLightPos = uLightPos;
	vLightPower = uLightPower;
	vAmbient = uAmbient;
}
