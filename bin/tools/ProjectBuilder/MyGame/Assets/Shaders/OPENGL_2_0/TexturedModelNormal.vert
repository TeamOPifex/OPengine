attribute vec3 aPosition;
attribute vec2 aUV;
attribute vec3 aNormal;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj;

varying vec3 vNormal; 
varying vec2 vUV;
varying vec3 vLightDirection;

void main() {	
	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV; 
	vNormal = aNormal;
	vLightDirection = vec3(0,1,0);
}
