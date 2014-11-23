#version 330 core

in vec3 aPosition;
in vec2 aUV;
out vec2 vTexCoord;

uniform mat4 uWorld;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;
	
	vTexCoord = aUV; 
}
