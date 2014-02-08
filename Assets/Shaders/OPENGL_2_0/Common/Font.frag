varying vec2 vTexCoord; 

uniform sampler2D uColorTexture;
uniform vec4 uColor;

void main() {
	vec4 tex = texture2D(uColorTexture, vTexCoord);	
	gl_FragColor = tex * uColor;
	gl_FragColor.a = tex.r;
}