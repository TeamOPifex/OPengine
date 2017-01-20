#version 330 core

out vec4 FragColor;
in vec3 vPos;

uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uGbufferAlbedoSpec;
uniform sampler2D uLightBuffer;
uniform sampler2D uSSAOBuffer;

//uniform vec3 viewPos;


void main()
{             

	vec2 tex_coord = gl_FragCoord.xy/vec2(1920, 1080);

    // Retrieve data from gbuffer
    vec3 FragPos = texture(uGbufferPosition, tex_coord).rgb;
    vec3 Diffuse = texture(uGbufferAlbedoSpec, tex_coord).rgb;
    vec3 Normal = texture(uGbufferNormal, tex_coord).rgb;
    float Specular = texture(uGbufferAlbedoSpec, tex_coord).a;
    vec3 Light = texture(uLightBuffer, tex_coord).rgb;
    float AmbientOcclusion = texture(uSSAOBuffer, tex_coord).r;
        
    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
   // if(draw_mode == 1)
        FragColor = vec4(AmbientOcclusion * vec3(0.5) * Diffuse, 1.0);
    //else if(draw_mode == 2)
    //    FragColor = vec4(FragPos, 1.0);
   // else if(draw_mode == 3)
    //    FragColor = vec4(Normal, 1.0);
    //else if(draw_mode == 4)
    //    FragColor = vec4(Diffuse, 1.0);
    //else if(draw_mode == 5)
    //	   FragColor = vec4(vec3(Specular), 1.0);

	//float d = texture(uGbufferDepth, tex_coord).r;
	//FragColor = vec4(d,d,d, 1.0);
	//FragColor = vec4(AmbientOcclusion, 1.0);
}