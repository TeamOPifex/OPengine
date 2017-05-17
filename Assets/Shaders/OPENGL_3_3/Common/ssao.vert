#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;

uniform float uAspectRatio;
uniform float uTanHalfFOV;

out vec2 vUV;
out vec2 vViewRay;

void main()
{
    gl_Position = vec4(aPosition, 1.0f);
    vUV = aUV;
    vViewRay.x = aPosition.x * uAspectRatio * uTanHalfFOV;
    vViewRay.y = aPosition.y * uTanHalfFOV;
}