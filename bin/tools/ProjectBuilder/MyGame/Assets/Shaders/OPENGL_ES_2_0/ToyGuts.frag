uniform sampler2D uColorTexture;

varying lowp vec2 vUV;

void main(void)
{
	gl_FragColor = texture2D(
		uColorTexture,
		vUV
	);
	if(gl_FragColor.a <= 0.0) discard;
}