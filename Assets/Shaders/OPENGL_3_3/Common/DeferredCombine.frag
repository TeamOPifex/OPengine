#version 330 core

out vec4 FragColor;
in vec3 vPos;

uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uGbufferAlbedoSpec;
uniform sampler2D uLightBuffer;
uniform sampler2D uSSAOBuffer;

//uniform vec3 viewPos;
uniform bool uUseSSAO;
uniform vec2 uScreenSize;

uniform vec3 uLightPos;

void main()
{             

	vec2 tex_coord = gl_FragCoord.xy / uScreenSize;//vec2(1280, 720);

    // Retrieve data from gbuffer
    vec3 FragPos = texture(uGbufferPosition, tex_coord).rgb;
    vec3 Diffuse = texture(uGbufferAlbedoSpec, tex_coord).rgb;
    vec3 Normal = texture(uGbufferNormal, tex_coord).rgb;
    float Shadow = texture(uGbufferNormal, tex_coord).a;
    float Specular = texture(uGbufferAlbedoSpec, tex_coord).a;
    vec3 Light = texture(uLightBuffer, tex_coord).rgb;
    float AmbientOcclusion = texture(uSSAOBuffer, tex_coord).r;
	if(!uUseSSAO) {
		AmbientOcclusion = 1.0;
	}
	
    vec3 lightColor = vec3(1.0);
	vec3 ambient = Diffuse * 0.5;
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 lighting = (ambient + (1.0 - Shadow) * (diffuse)) * Diffuse * AmbientOcclusion;

	//vec3 final = Light * Diffuse * vec3(AmbientOcclusion) * (1.0 - Shadow);

    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
   // if(draw_mode == 1)
        FragColor = vec4(lighting, 1.0);
    //else if(draw_mode == 2)
    //    FragColor = vec4(FragPos, 1.0);
   // else if(draw_mode == 3)
    //    FragColor = vec4(Normal, 1.0);
    //else if(draw_mode == 4)
       // FragColor = vec4(Diffuse, 1.0);
    //else if(draw_mode == 5)
    //	   FragColor = vec4(vec3(Specular), 1.0);

	//float d = texture(uGbufferDepth, tex_coord).r;
	//FragColor = vec4(d,d,d, 1.0);
	//FragColor = vec4(AmbientOcclusion, 1.0);
}