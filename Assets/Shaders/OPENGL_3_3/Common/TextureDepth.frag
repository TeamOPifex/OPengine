#version 330 core

in vec2 vUV;

uniform sampler2D uColorTexture;

out vec4 FragColor;

float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane = 1000.0;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
	float depthValue = texture(uColorTexture, vUV).r;
	FragColor = vec4(vec3(LinearizeDepth(depthValue)),1);
}
