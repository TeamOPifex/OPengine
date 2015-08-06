varying vec2 vUV;
varying vec4 vShadowCoord; // FragPosLightSpace
varying vec3 vNormal;
varying vec3 vFragPos;

uniform sampler2D uColorTexture;
uniform sampler2D uShadow;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(uShadow, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    //float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;




    if(projCoords.z > 1.0) {
        return 0.0;
    }

    return shadow;
}

void main(){

    vec3 color = texture2D(uColorTexture, vUV).rgb;

    vec3 normal = normalize(vNormal);

    vec3 lightColor = vec3(1.0);

    // Ambient
    vec3 ambient = 0.15 * color;

    // Diffuse
    vec3 lightDir = normalize(uLightPos - vFragPos);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // Calculate shadow
    float shadow = ShadowCalculation(vShadowCoord, lightDir, normal);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    gl_FragColor = vec4(lighting, 1.0);
}
