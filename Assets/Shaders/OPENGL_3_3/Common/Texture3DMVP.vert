#version 330 core

in vec3 aPosition;
in vec2 aUV;

uniform mat4 uMVP;

out vec2 vUV;

void main() {
	gl_Position = uMVP * vec4(aPosition,1);

	vUV = vec2(aUV.x, 1.0 - aUV.y);// aUV; //(aUV * vec2(1.0, -1.0) + vec2(0.0,1.0));
}
