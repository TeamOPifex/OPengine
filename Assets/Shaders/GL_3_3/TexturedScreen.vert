#version 330 core

attribute vec3 aPosition;
attribute vec2 aUV;
varying lowp vec2 vTexCoord;

uniform mat4 uWorld;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;
	
	vTexCoord = aUV; 
}
