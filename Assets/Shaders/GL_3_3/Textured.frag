#version 330 core
#define PI 3.14159

varying lowp vec2 vUV; 
uniform sampler2D uTexture; 

void main() {
	lowp float d = 0.001;
	lowp vec4 color = vec4(0.0);
	int samples = 0;

	float dt = (2 * PI) / 9.0; 
	for(int i = 0; i < 9; i++){
		lowp vec2 off = vec2(cos(dt * i), sin(dt * i));
		color += texture2D(uTexture, vUV + (off * d));
	}
	/*
	for(int i = 1; i >= -1; i--)
		for(int j = 1; j >= -1; j--){
			lowp float x = float(j) - 1.0;
			lowp float y = float(i) - 1.0;
			lowp vec2 off = vec2(x * d, y * d);
			color += texture2D(uTexture, vUV + off);
			++samples;
		}*/
	color /= 9.0;
	gl_FragColor = color;
}
