#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTangent;
in vec2 aUV;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 

varying out vec2 TexCoordOut; 
varying out vec3 NormalOut; 
varying out vec3 TangentOut; 

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	TexCoordOut = aUV; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	NormalOut = normalize(rotScl * aNormal); 
	TangentOut = normalize(rotScl * aTangent); 
}
