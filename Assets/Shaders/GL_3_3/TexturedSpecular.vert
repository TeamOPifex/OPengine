#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTangent;
in vec2 aUV;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 

varying vec2 vUV; 
varying vec3 vNormal; 
varying vec3 vTangent; 
varying vec3 vBullshit;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	vNormal = normalize(rotScl * aNormal); 
	vTangent = normalize(rotScl * aTangent);
	vBullshit = vec3(0, 0, 1);
}
