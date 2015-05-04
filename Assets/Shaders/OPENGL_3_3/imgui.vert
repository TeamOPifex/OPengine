#version 330 core

in vec3 aPosition;
in vec4 aColor;

uniform mat4 uProj;

out vec4 vColor;

void main() {
	gl_Position = uProj * vec4(aPosition,1);

	vColor = aColor;
}
