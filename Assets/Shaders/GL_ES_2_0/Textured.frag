varying highp vec2 TexCoordOut; uniform sampler2D Texture; void main() {	gl_FragColor = texture2D(Texture, TexCoordOut);	}
