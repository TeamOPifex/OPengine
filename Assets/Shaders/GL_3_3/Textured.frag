#version 330 core

varying vec2 vUV; 
uniform sampler2D uTexture; 

void main() {
	float d = 0.005;
	vec4 color = vec4(0.0);
	int samples = 0;
	for(int i = 3; i >= -2; i--)
		for(int j = 3; j >= -2; j--){
			float x = float(j) - 1.0;
			float y = float(i) - 1.0;
			vec2 off = vec2(x * d, y * d);
			color += texture2D(uTexture, vUV + off);
			++samples;
		}
	color /= float(samples);
	gl_FragColor = color;
}
