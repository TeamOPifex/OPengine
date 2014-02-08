#version 330 core
varying lowp vec3 vColor;

void main(void){
	gl_FragColor = vec4(vColor, 1);
}
