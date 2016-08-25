#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uProj;

out vec2 vUV;

void main() {
	vec4 screenPos = uProj * uWorld * vec4(aPosition,1);
	gl_Position = screenPos;

	vUV = aUV;
}
