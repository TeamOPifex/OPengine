#version 330 core

out vec4 FragColor;

in vec4 vPos;
in mat4 vInvViewProj;

//uniform vec3 viewPos;
uniform sampler2D uGbufferDepth;
uniform mat4 uInvertViewProjection;


void main()
{	
	vec2 screenPos = vPos.xy / vPos.w;


	vec2 texCoord = screenPos.xy / vec2(1920, 1080);

    //vec2 texCoord = 0.5f * (vec2(screenPos.x, -screenPos.y) + 1);
    // texCoord -=halfPixel;

	float depthVal = texture(uGbufferDepth, texCoord).r;    

    vec4 position;
    position.xy = texCoord.xy;
    position.z = depthVal;
    position.w = 1.0f;

    //transform to world space
    position = position * uInvertViewProjection;
    position /= position.w;
	
	vec4 lightPosition = vec4(0, 0, 0, 1);
    vec4 lightVector = lightPosition - position;
	float lightRadius = 5.0;
    float attenuation = clamp(1.0f - length(lightVector) / lightRadius, 0, 1.0); 

	FragColor = attenuation * vec4(1.0);
}