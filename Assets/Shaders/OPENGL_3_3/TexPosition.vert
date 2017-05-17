#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uWorld;
uniform vec4 uUVScale;

uniform float uAspectRatio;
uniform float uTanHalfFOV;

out vec2 vUV;
out vec2 vViewRay;

void main() {



	float width = uUVScale.z - uUVScale.x;
	float height = uUVScale.w - uUVScale.y;
	vUV = vec2((1.0 - aUV.x) * width, (1.0 - aUV.y) * height);
   // vUV = (aPosition.xy + vec2(1.0)) / 2.0;


	gl_Position = uWorld * vec4(aPosition,1);

    vViewRay.x = aPosition.x * uAspectRatio * uTanHalfFOV;
    vViewRay.y = aPosition.y * uTanHalfFOV;
}
