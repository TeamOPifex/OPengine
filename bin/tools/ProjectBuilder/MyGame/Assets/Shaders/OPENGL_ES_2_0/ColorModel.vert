attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aColor;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightDir; 
uniform float uLightPower;

varying vec4 vColor; 
varying vec3 vNormal; 
varying vec3 vLightDirection; 
varying float vLightPower;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	vNormal = ( uView * uWorld * vec4(aNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	vColor = vec4(aColor, 1.0);	
	vLightDirection = uLightDir;
	vLightPower = uLightPower;
}
