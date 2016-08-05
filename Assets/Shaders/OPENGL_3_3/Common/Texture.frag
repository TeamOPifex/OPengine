#version 330 core

in vec2 vUV;

uniform sampler2D uColorTexture;

out vec4 FragColor;

void main() {
	FragColor = texture(uColorTexture, vUV);
	//FragColor = vec4(1,0,0,1);
}
