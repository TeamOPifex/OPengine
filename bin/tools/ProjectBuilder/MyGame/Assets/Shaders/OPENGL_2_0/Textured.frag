#define PI 3.14159
#define SAMP 8.0

varying vec2 vUV; 
uniform sampler2D uTexture; 

void main() {
	float d = 0.005;
	vec4 color = vec4(0.0);
	int samples = 0;

	float dt = (2.0 * PI) / SAMP; 
	for(int i = 0; i < int(SAMP); i++){
		vec2 off = vec2(cos(dt * float(i)), sin(dt * float(i)));
		color += texture2D(uTexture, vUV + (off * d));
	}

	color /= SAMP;
	gl_FragColor = color;
}
