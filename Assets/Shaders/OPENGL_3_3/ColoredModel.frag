#version 310 es

layout(location = 0) in highp vec3 vColor;

layout(location = 0) out highp vec4 FragColor;

void main() {
	FragColor = vec4(vColor, 1);
}
