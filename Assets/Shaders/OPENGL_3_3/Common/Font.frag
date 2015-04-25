#version 330 core

in vec2 vUV;

uniform sampler2D uColorTexture;
uniform vec4 uColor;

out vec4 FragColor;

void main() {
	vec4 tex = texture(uColorTexture, vUV);
	FragColor = tex * uColor;
	FragColor.a = tex.r;
}