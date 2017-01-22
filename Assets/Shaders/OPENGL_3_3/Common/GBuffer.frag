#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 vWorldPos;
in vec2 vUV;
in vec3 vFragPos;
in vec3 vNormal;
in vec4 vFragPosLightSpace;

uniform sampler2D uAlbedoMap;
uniform sampler2D uSpecularMap;
uniform sampler2D uShadow;

uniform vec3 uLightPos;

const float NEAR = 0.01;
const float FAR = 50.0f;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = vFragPosLightSpace.xyz / vFragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadow, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vWorldPos);
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);

    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadow, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadow, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition.xyz = vFragPos;
    gPosition.a = LinearizeDepth(gl_FragCoord.z); // Divide by far to store depth in range 0.0 - 1.0
    // Also store the per-fragment normals into the gbuffer
    gNormal.xyz = normalize(vNormal);
    // And the diffuse per-fragment color
	gNormal.a = ShadowCalculation(vFragPosLightSpace);
    gAlbedoSpec.rgb = texture(uAlbedoMap, vec2(vUV.x, 1.0 - vUV.y)).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 1.0;
}