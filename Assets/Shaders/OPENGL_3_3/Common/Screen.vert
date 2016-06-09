#version 330 core

in vec2 aPosition;

out vec2 vUV;

void main() {
	gl_Position = vec4(aPosition,0,1);

	vUV = aPosition;
}
