#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uProj;
uniform mat4 uView;
uniform vec2 uOffset;
uniform vec2 uSize;

out vec2 vUV;

void main() {
	vec4 screenPos = uProj * uView * uWorld * vec4(aPosition,1);
	gl_Position = screenPos;

    vec2 uv = vec2(aUV.x, 1.0 - aUV.y);

	vUV = (uv * uSize) + uOffset;

    vUV = aPosition.xy;
}
