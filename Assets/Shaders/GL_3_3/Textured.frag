#version 330 core
#define PI 3.14159
#define SAMP 8.0

varying lowp vec2 vUV; 
uniform sampler2D uTexture; 

void main() {
	lowp float d = 0.005;
	lowp vec4 color = vec4(0.0);
	int samples = 0;

	float dt = (2 * PI) / SAMP; 
	for(int i = 0; i < int(SAMP); i++){
		lowp vec2 off = vec2(cos(dt * i), sin(dt * i));
		color += texture2D(uTexture, vUV + (off * d));
	}

	color /= SAMP;
	gl_FragColor = color;
}
