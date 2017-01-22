#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D uAlbedoMap;
uniform sampler2D uShadow;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uColor;
uniform float uColorAmount;

// uniform bool uShadows;

out vec4 FragColor;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadow, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(uLightPos - fs_in.FragPos);
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

vec3 texColor(vec2 uv) {
	vec3 result = texture(uAlbedoMap, uv).rgb;
	float dist = 0.00025;
	result += texture(uAlbedoMap, uv + vec2(0.0, dist)).rgb;
	result += texture(uAlbedoMap, uv + vec2(0.0, -dist)).rgb;
	result += texture(uAlbedoMap, uv + vec2(dist, 0.0)).rgb;
	result += texture(uAlbedoMap, uv + vec2(-dist, 0.0)).rgb;
	result += texture(uAlbedoMap, uv + vec2(-dist, -dist)).rgb;
	result += texture(uAlbedoMap, uv + vec2(dist, dist)).rgb;
	result += texture(uAlbedoMap, uv + vec2(-dist, dist)).rgb;
	result += texture(uAlbedoMap, uv + vec2(dist, -dist)).rgb;
	return result / 9.0;
}

void main()
{
    vec3 color = texColor(fs_in.TexCoords);// texture(uAlbedoMap, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);

	float belowY = 0;
	if(fs_in.FragPos.y < 0) {
		belowY = -1 * max(-1.0, fs_in.FragPos.y / 3.0);
	}

    vec3 lightColor = vec3(1.0);

    // Ambient
    vec3 ambient = 0.5 * color;


    // Diffuse
    vec3 lightDir = normalize(uLightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;


    // Specular
    vec3 viewDir = normalize(uViewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // Calculate shadow
    //float shadow = shadows ? ShadowCalculation(fs_in.FragPosLightSpace) : 0.0;
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	//shadow = 0;
    vec3 lighting = (ambient + (1.0 - shadow * 0.75) * (diffuse + specular)) * color;
	
	vec3 finalColor = (1.0 - uColorAmount) * lighting + (uColorAmount * uColor);

    FragColor = vec4(finalColor, 1.0f) * (1.0 - belowY);
}
