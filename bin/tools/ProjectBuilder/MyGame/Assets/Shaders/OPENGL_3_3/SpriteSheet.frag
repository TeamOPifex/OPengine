#version 330 core

in vec2 vTexCoord; 

uniform sampler2D uColorTexture;

uniform vec2 uTexCoordStart;
uniform vec2 uTexCoordSize;
uniform vec4 uTint;
uniform float uAlpha;
uniform vec3 uColor;

out vec4 color;

void main() {
	vec4 tint = uTint.a == 0 ? vec4(1) : uTint;
	color = texture(uColorTexture, uTexCoordStart + vTexCoord * uTexCoordSize) * tint;
	color.a *= uAlpha;
}
