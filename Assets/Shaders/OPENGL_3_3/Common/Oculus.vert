#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in float timewarpLerpFactor;
layout (location = 2) in float vignette;
layout (location = 3) in vec2 texCoord0;
layout (location = 4) in vec2 texCoord1;
layout (location = 5) in vec2 texCoord2;

uniform vec2 eyeToSourceUVscale;
uniform vec2 eyeToSourceUVoffset;
uniform mat4 eyeRotationStart;
uniform mat4 eyeRotationEnd;

out vec2 oTexCoord0;
out vec2 oTexCoord1;
out vec2 oTexCoord2;
out float oVignette;

vec2 timeWarpTexCoord(in vec2 texCoord, in mat4 rotMat) {

    vec3 transformed = (rotMat * vec4(texCoord.xy, 1, 1)).xyz;

    vec2 flattened = transformed.xy / transformed.z;

    return eyeToSourceUVscale * flattened + eyeToSourceUVoffset;
}

void main() {

    mat4 lerpedEyeRot = eyeRotationStart * (1 - timewarpLerpFactor) + eyeRotationEnd * timewarpLerpFactor;

    oTexCoord0 = timeWarpTexCoord(texCoord0, lerpedEyeRot);
    oTexCoord1 = timeWarpTexCoord(texCoord1, lerpedEyeRot);
    oTexCoord2 = timeWarpTexCoord(texCoord2, lerpedEyeRot);

    gl_Position = vec4(position.x, position.y, 0.5, 1);

    oVignette = vignette;
}