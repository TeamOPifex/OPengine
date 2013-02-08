varying in vec2 TexCoordOut; uniform sampler2D uTexture; void main() {	gl_FragColor = texture2D(uTexture, TexCoordOut);}
