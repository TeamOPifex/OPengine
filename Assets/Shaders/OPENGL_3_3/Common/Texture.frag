#version 330 core

in vec2 vUV;

uniform sampler2D uColorTexture;

out vec4 FragColor;

void main() {
	FragColor = vec4(1,0,0,1) + texture(uColorTexture, vUV);
	//FragColor = vec4(1,0,0,1);
}
