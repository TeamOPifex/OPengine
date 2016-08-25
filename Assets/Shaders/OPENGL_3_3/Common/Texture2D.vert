#version 330 core

in vec3 aPosition;
in vec2 aUV;

uniform mat4 uWorld;
uniform vec4 uUVScale;

out vec2 vUV;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;
	
	float width = uUVScale.z - uUVScale.x;
	float height = uUVScale.w - uUVScale.y;
	vUV = vec2(aUV.x * width, aUV.y * height);
}
