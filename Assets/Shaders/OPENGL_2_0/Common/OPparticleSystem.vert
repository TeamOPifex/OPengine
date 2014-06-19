attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aUV;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 

varying vec2 vUV; 
varying vec3 vNormal;
varying vec3 vTangent; 

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	vNormal = normalize(rotScl * aNormal);
	vTangent = normalize(rotScl * vec3(1, 0, 0));
}
