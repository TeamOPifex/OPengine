varying vec2 vUV;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;
uniform vec2 uTexCoordScale;
uniform vec2 uSpriteOffset;
uniform vec4 uTint;

void main() {
	vec2 uv = vUV * uTexCoordScale + uSpriteOffset;
	vec4 color = texture2D(uColorTexture, uv);
	
	if(color.a == 0.0) discard;

	gl_FragColor = color;
	gl_FragColor *= uTint;
}
