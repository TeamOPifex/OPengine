varying in vec2 TexCoordOut; uniform sampler2D uColorTexture; uniform float uAlpha;void main() {	gl_FragColor = texture2D(uColorTexture, TexCoordOut);	gl_FragColor.a *= uAlpha;}
