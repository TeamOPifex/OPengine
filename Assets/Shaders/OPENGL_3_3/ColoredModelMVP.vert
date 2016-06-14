#version 330 core

in vec3 aPosition;
in vec3 aColor;

uniform mat4 uMVP;

out vec3 vColor;

void main() {

	gl_Position = uMVP * vec4(aPosition, 1.0);

	vColor = aColor;
}
