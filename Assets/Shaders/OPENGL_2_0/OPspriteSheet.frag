varying vec2 vUV;

uniform sampler2D uColorTexture;

uniform vec2 uOffset;
uniform vec2 uSize;

void main(){
	vec2 uv = vUV * uSize;
	gl_FragColor = texture(uColorTexture, uOffset +  uv);
}
