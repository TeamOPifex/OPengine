#version 300 es

in highp vec2 vUV;

uniform sampler2D uColorTexture;
out highp vec4 FragColor;

highp float LinearizeDepth(highp float depth)
{
	highp float near_plane = 0.1;
	highp float far_plane = 50.0;
    highp float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
	highp float depthValue = texture(uColorTexture, vUV).r;
	FragColor = vec4(vec3(LinearizeDepth(depthValue)),1.0);
}
