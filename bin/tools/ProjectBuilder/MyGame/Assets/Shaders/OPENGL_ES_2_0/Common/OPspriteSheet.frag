varying lowp vec2 vUV;

uniform sampler2D uColorTexture;

void main(){
	gl_FragColor = texture2D(uColorTexture, vUV);
	if(gl_FragColor.a <= 0.5)
		discard;
}
