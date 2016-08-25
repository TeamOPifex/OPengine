#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform vec4 uUVScale;

out vec2 vUV;

void main() {


	float width = uUVScale.z - uUVScale.x;
	float height = uUVScale.w - uUVScale.y;
	vUV = vec2(aUV.x * width, aUV.y * height);
	//vUV = vec2(aUV.x, aUV.y);
    //vUV = vec2(aPosition.x * 2.0, aPosition.y * 2.0);


	gl_Position = uWorld * vec4(aPosition,1);
}
