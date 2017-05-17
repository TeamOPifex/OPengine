#version 330 core
out float FragColor;
in vec2 vUV;
in vec2 vViewRay;

//uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uNoise;
uniform sampler2D uGbufferDepth;

uniform vec3 samples[64];
uniform mat4 uProj;

uniform int kernelSize = 32;
uniform float radius = 1.0;

float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1920.0f/4.0f, 1200.0f/4.0f);

float CalcViewZ(vec2 Coords)
{
    float Depth = texture(uGbufferDepth, Coords).x;
    float ViewZ = uProj[3][2] / (2 * Depth -1 - uProj[2][2]);
    return ViewZ;
}

void main()
{

    float ViewZ = CalcViewZ(vUV);

    float ViewX = vViewRay.x * ViewZ;
    float ViewY = vViewRay.y * ViewZ;

    vec3 fragPos = vec3(ViewX, ViewY, ViewZ);

    //vec3 fragPos = texture(uGbufferPosition, vUV).xyz; // in View Space

    // Get input for SSAO algorithm
    vec3 normal = texture(uGbufferNormal, vUV).rgb;
    vec3 randomVec = texture(uNoise, vUV * noiseScale).xyz;

    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;

    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sample = TBN * samples[i]; // From tangent to view-space
        sample = fragPos + sample * radius; // view-space sample position
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = uProj * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        float sampleDepth = CalcViewZ(offset.xy); //texture(uGbufferPosition, offset.xy).z; // Get depth value of kernel sample
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    
	FragColor = occlusion;
}