#version 330 core

in vec3 aPosition;
in vec2 aUV;

uniform mat4 uWorld;

out vec2 vUV;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;

	vUV = vec2(aUV.x, 1.0 - aUV.y);
}
