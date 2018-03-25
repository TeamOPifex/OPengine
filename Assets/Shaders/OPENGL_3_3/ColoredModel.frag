#version 330 core

in highp vec3 vColor;

out highp vec4 FragColor;

void main() {
	FragColor = vec4(vColor, 1);
}
