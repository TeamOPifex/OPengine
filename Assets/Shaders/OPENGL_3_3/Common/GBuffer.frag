#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 vUV;
in vec3 vFragPos;
in vec3 vNormal;

uniform sampler2D uAlbedoMap;
uniform sampler2D uSpecularMap;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = vFragPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(uAlbedoMap, vUV).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 1.0 + texture(uSpecularMap, vUV).r;
}