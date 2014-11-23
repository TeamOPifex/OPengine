varying lowp vec2 vTexCoord; 

uniform sampler2D uColorTexture;

uniform lowp vec2 uTexCoordStart;
uniform lowp vec2 uTexCoordSize;
uniform lowp vec4 uTint;
uniform lowp float uAlpha;
uniform lowp vec3 uColor;

void main() {
	lowp vec4 tint = uTint.a == 0.0 ? vec4(1) : uTint;
	gl_FragColor = texture2D(uColorTexture, uTexCoordStart + vTexCoord * uTexCoordSize) * tint;
	gl_FragColor.a *= uAlpha;
}
