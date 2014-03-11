varying vec2 vUV;

uniform sampler2D uColorTexture;

void main(){
	gl_FragColor = texture2D(uColorTexture, vUV);
}