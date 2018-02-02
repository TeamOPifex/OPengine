#version 310 es

layout(location = 0) in highp vec2 vUV;
uniform sampler2D uTexture; 

layout(location = 0) out highp vec4 FragColor;

void main() {
	highp float PI = 3.14159;
	lowp float SAMP = 8.0;

	highp float d = 0.005;
	lowp vec4 color = vec4(0.0);
	int samples = 0;

	highp float dt = ((2.0 * PI) / SAMP); 
	for(int i = 0; i < int(SAMP); i++) {
		highp float v = dt * float(i);
		highp vec2 off = vec2(
			cos(v), 
			sin(v));

		highp vec2 uv = vUV + (off * d);
		color += texture(uTexture, uv);
	}

	color /= SAMP;
	FragColor  = color;
}
