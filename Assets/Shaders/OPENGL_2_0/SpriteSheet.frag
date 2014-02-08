varying vec2 vTexCoord; 

uniform sampler2D uColorTexture;

uniform vec2 uTexCoordStart;
uniform vec2 uTexCoordSize;
uniform vec4 uTint;
uniform float uAlpha;
uniform vec3 uColor;

void main() {
	vec4 tint = uTint.a == 0.0 ? vec4(1) : uTint;
	gl_FragColor = texture2D(uColorTexture, uTexCoordStart + vTexCoord * uTexCoordSize) * tint;
	gl_FragColor.a *= uAlpha;
}
