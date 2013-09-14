#version 330 core
attribute vec3 aVertexPosition;
attribute vec3 aColor;

varying lowp vec3 vColor;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main(void){
	gl_Position = uProj * uWorld * vec4(aVertexPosition.xyz, 1);

	vColor = aColor;
}