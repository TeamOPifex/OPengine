#define PI 3.14159

varying lowp vec2 vUV; 
uniform sampler2D uTexture; 

void main() {
	lowp float d = 0.005;
	lowp vec4 color = vec4(0.0);
	int samples = 0;

	lowp float dt = (2.0 * PI) / 9.0; 
	for(int i = 0; i < 9; i++){
		lowp vec2 off = d * vec2(cos(dt * float(i)), sin(dt * float(i)));
		
		color += texture2D(uTexture, vUV + off);
	}
	
	color /= 9.0;
	gl_FragColor = color;
}