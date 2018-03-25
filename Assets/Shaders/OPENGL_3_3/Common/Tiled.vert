#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vUV;

void main() {
	vUV = aUV;
	gl_Position = uProj * uView * uWorld * vec4(aPosition.xy, 0.0, 1.0);
}
