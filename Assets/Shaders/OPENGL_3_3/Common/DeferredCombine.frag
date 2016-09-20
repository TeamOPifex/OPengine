#version 330 core

out vec4 FragColor;
in vec3 vPos;

uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uGbufferAlbedoSpec;
uniform sampler2D uLightBuffer;

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
        
    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
   // if(draw_mode == 1)
        FragColor = vec4(Diffuse * Light, 1.0);
    //else if(draw_mode == 2)
    //    FragColor = vec4(FragPos, 1.0);
   // else if(draw_mode == 3)
    //    FragColor = vec4(Normal, 1.0);
    //else if(draw_mode == 4)
    //    FragColor = vec4(Diffuse, 1.0);
    //else if(draw_mode == 5)
    //	   FragColor = vec4(vec3(Specular), 1.0);
}