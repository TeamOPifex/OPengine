varying lowp vec2 TexCoordOut; uniform sampler2D uColorTexture; uniform lowp float uAlpha;void main() {	gl_FragColor = texture2D(uColorTexture, TexCoordOut);	gl_FragColor.a *= uAlpha;}
