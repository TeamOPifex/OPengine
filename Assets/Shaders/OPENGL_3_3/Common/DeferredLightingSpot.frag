#version 330 core

out vec4 FragColor;

in vec4 vPos;
in mat4 vInvViewProj;

//uniform vec3 viewPos;
uniform sampler2D uGbufferDepth;
uniform sampler2D uGbufferPosition;
uniform mat4 uInvertViewProjection;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform float uLightRadius;


vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
	vec3 pointLightPos = vec3(0,0,0);
    vec3 LightDirection = WorldPos - pointLightPos;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = vec4(1.0, 1.0, 1.0, 1.0); //CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal);

	float attenConst = 0.0;
	float attenLinear = 0.01;
	float attenExp = 0.3;

    float Attenuation =  attenConst +
                         attenLinear * Distance +
                         attenExp * Distance * Distance;

    Attenuation = max(1.0, Attenuation);

    return Color / Attenuation;
}

vec4 CalcPointLightLinear(vec3 WorldPos)
{
    vec3 LightDirection = WorldPos - uLightPos;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);
	
    float Attenuation =  clamp(1.0 - (Distance / uLightRadius), 0.0, 1.0f);

    return vec4(uLightColor * Attenuation, 1.0);
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / vec2(1920, 1080);//gScreenSize;
}

void main()
{	
    vec2 TexCoord = CalcTexCoord();
    vec3 WorldPos = texture(uGbufferPosition, TexCoord).xyz;
	FragColor = CalcPointLightLinear(WorldPos);
}