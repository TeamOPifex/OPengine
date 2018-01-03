#version 300 es

in highp vec3 FragPos;
in highp vec3 Normal;
in highp vec2 TexCoords;
in highp vec4 FragPosLightSpace;

uniform sampler2D uAlbedoMap;
uniform sampler2D uShadow;

uniform highp vec3 uLightPos;
uniform highp vec3 uViewPos;
uniform highp vec3 uColor;
uniform highp float uColorAmount;

// uniform bool uShadows;

out highp vec4 FragColor;


highp float ShadowCalculation(highp vec4 fragPosLightSpace)
{
    // perform perspective divide
    highp vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    highp float closestDepth = texture(uShadow, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    highp float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    highp vec3 normal = normalize(Normal);
    highp vec3 lightDir = normalize(uLightPos - FragPos);
    highp float bias = max(0.00005 * (1.0 - dot(normal, lightDir)), 0.000005);

    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    highp float shadow = 0.0;
    highp ivec2 texSize = textureSize(uShadow, 0);
    highp float texSizeX = float(texSize.x);
    highp float texSizeY = float(texSize.y);
    highp vec2 texelSize = vec2(1.0 / texSizeX, 1.0 / texSizeY);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            highp float pcfDepth = texture(uShadow, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

highp vec3 texColor(highp vec2 uv) {
	highp vec3 result = texture(uAlbedoMap, uv).rgb;
	highp float dist = 0.00025;
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
    highp vec3 color = texColor(TexCoords);// texture(uAlbedoMap, TexCoords).rgb;
    highp vec3 normal = normalize(Normal);

	highp float belowY = 0.0;
	if(FragPos.y < 0.0) {
		belowY = -1.0 * max(-1.0, FragPos.y / 3.0);
	}

    highp vec3 lightColor = vec3(1.0);

    // Ambient
    highp vec3 ambient = 0.5 * color;


    // Diffuse
    highp vec3 lightDir = normalize(uLightPos - FragPos);
    highp float diff = max(dot(lightDir, normal), 0.0);
    highp vec3 diffuse = diff * lightColor;


    // Specular
    highp vec3 viewDir = normalize(uViewPos - FragPos);
    highp vec3 reflectDir = reflect(-lightDir, normal);
    highp float spec = 0.0;
    highp vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    highp vec3 specular = spec * lightColor;

    // Calculate shadow
    //float shadow = shadows ? ShadowCalculation(FragPosLightSpace) : 0.0;
    highp float shadow = ShadowCalculation(FragPosLightSpace);
	//shadow = 0;   
    highp vec3 lighting = (ambient + (1.0 - shadow * 0.75) * (diffuse + specular)) * color;
	
	highp vec3 finalColor = (1.0 - uColorAmount) * lighting + (uColorAmount * uColor);

    FragColor = vec4(finalColor, 1.0f) * (1.0 - belowY);
}
