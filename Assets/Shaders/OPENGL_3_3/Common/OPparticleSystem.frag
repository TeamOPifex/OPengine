#version 330 core

in vec2 vUV;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;

uniform vec2 uTexCoordScale;
uniform vec2 uSpriteOffset;
uniform vec4 uTint;

out vec4 FragColor;

void main() {
	vec2 uv = (vUV * uTexCoordScale) + uSpriteOffset;
	vec4 color = texture(uColorTexture, uv);

	FragColor = color;
	FragColor *= uTint;
}
