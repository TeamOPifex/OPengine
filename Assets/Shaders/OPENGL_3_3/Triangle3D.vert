#version 330 core
attribute vec3 aVertexPosition;
attribute vec3 aColor;

varying lowp vec3 vColor;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main(void){
    vec4 worldPos = uWorld * vec4(aVertexPosition, 1.0);
    vec4 viewPos = uView * worldPos;
    vec4 screenPos = uProj * viewPos;
	gl_Position = screenPos;

	vColor = aColor;
}