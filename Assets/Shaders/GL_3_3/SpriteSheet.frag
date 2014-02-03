#version 330 core

varying lowp vec2 vTexCoord; 

uniform sampler2D uColorTexture;

uniform lowp vec2 uTexCoordStart;
uniform lowp vec2 uTexCoordSize;
uniform vec4 uTint;
uniform float uAlpha;

uniform highp vec3 uColor;

void main() {
	vec4 tint = uTint.a == 0 ? vec4(1) : uTint;
	gl_FragColor = texture2D(uColorTexture, uTexCoordStart + vTexCoord * uTexCoordSize) * tint;
	gl_FragColor.a *= uAlpha;
}
