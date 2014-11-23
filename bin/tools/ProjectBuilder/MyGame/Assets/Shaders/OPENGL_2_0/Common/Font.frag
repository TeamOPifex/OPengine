varying vec2 vUV; 

uniform sampler2D uColorTexture;
uniform vec4 uColor;

void main() {
	vec4 tex = texture2D(uColorTexture, vUV);	
	gl_FragColor = tex * uColor;
	gl_FragColor.a = tex.r;
}