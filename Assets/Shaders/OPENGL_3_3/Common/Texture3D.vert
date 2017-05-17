#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vUV;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos;

	vUV = vec2(aUV.x, 1.0 - aUV.y);
	//vUV = aUV; //(aUV * vec2(1.0, -1.0) + vec2(0.0,1.0));
}
