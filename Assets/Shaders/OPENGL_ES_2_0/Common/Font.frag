varying lowp vec2 vTexCoord; 

uniform sampler2D uColorTexture;
uniform lowp vec4 uColor;

void main() {
	lowp vec4 tex = texture2D(uColorTexture, vTexCoord);	
	gl_FragColor = tex * uColor;
	gl_FragColor.a = tex.r;
}