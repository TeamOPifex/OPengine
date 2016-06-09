#version 330 core

in vec3 aPosition;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main(void){
    vec4 worldPos = uWorld * vec4(aPosition, 1.0);
    vec4 viewPos = uView * worldPos;
    vec4 screenPos = uProj * viewPos;
	gl_Position = screenPos;
}