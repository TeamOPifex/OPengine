#version 330 core

in vec3 aPosition;
in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uProj;
uniform mat4 uView;
uniform vec2 uOffset;
uniform vec2 uSize;

out vec2 vUV;

void main() {
	vec4 screenPos = uProj * uView * uWorld * vec4(aPosition,1);
	gl_Position = screenPos;
	
	vUV = (aUV * vec2(1.0, -1.0) + vec2(0.0,1.0)) * uSize + uOffset; 
}
