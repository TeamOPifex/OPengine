#version 330 core

in vec2 vUV;
in vec2 vViewRay;

uniform sampler2D uColorTexture;
uniform mat4 uProjScene;

out vec4 FragColor;

float CalcViewZ(vec2 Coords)
{
    float Depth = texture(uColorTexture, Coords).x;
    float ViewZ = uProjScene[3][2] / (2 * Depth -1 - uProjScene[2][2]);
    return ViewZ;
}

float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane = 50.0;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

vec3 PositionFromDepth(vec2 Coords)
{
    // Get the depth value for this pixel
    float z = texture(uColorTexture, Coords).x;

    // Get x/w and y/w from the viewport position
    float x = Coords.x * 2 - 1;
    float y = Coords.y * 2 - 1;

    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    vec4 vPositionVS = vProjectedPos * inverse(uProjScene);  
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;  
}

void main() {
    float ViewZ = CalcViewZ(vUV);

    float ViewX = vViewRay.x * ViewZ;
    float ViewY = vViewRay.y * ViewZ;

    vec3 fragPos = vec3(ViewX, ViewY, ViewZ);

	float depth = texture(uColorTexture, vUV).x;

	FragColor = vec4(PositionFromDepth(vUV), 1.0);
}
