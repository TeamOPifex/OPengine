#version 330 core

in vec3 aPosition;
in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vUV;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos;

	vUV = aUV; //(aUV * vec2(1.0, -1.0) + vec2(0.0,1.0));
}
