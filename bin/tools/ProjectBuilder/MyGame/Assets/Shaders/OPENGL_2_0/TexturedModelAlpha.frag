varying vec3 vNormal; 
varying vec2 vUV; 
varying vec3 vLightDirection;

uniform sampler2D uColorTexture; 
uniform vec3 uColor;

void main() {
	gl_FragColor = texture2D(uColorTexture, vUV);
	if(gl_FragColor.a <= 0.05)
		discard;
	gl_FragColor.rgb *= uColor;
}
