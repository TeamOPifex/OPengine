varying lowp vec3 vNormal; 
varying lowp vec2 vUV; 
varying lowp vec3 vLightDirection;

uniform sampler2D uColorTexture; 
uniform lowp vec3 uColor;

void main() {
	gl_FragColor = texture2D(uColorTexture, vUV);
	if(gl_FragColor.a <= 0.05)
		discard;
	gl_FragColor.rgb *= uColor;
}
