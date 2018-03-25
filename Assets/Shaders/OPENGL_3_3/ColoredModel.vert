#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

layout (std140) uniform MatrixBlock
{
	mat4 uWorld;
	mat4 uView;
	mat4 uProj;
};

out vec3 vColor;

void main() {

	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	gl_Position = (uProj * uView) * worldPos;

	vColor = aColor;
}
