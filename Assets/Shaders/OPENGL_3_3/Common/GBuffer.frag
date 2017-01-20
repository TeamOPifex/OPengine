#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 vUV;
in vec3 vFragPos;
in vec3 vNormal;

uniform sampler2D uAlbedoMap;
uniform sampler2D uSpecularMap;

const float NEAR = 0.01;
const float FAR = 50.0f;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition.xyz = vFragPos;
    gPosition.a = LinearizeDepth(gl_FragCoord.z); // Divide by far to store depth in range 0.0 - 1.0
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(uAlbedoMap, vec2(vUV.x, 1.0 - vUV.y)).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 1.0;
}