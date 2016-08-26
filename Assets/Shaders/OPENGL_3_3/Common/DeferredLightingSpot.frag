#version 330 core

out vec4 FragColor;
//in vec2 vUV;

uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uGbufferAlbedoSpec;

//uniform vec3 viewPos;


void main()
{             

	vec2 tex_coord = gl_FragCoord.xy/vec2(1920, 1080);

    // Retrieve data from gbuffer
    vec3 FragPos = texture(uGbufferPosition, tex_coord).rgb;
    vec3 Normal = texture(uGbufferNormal, tex_coord).rgb;
    vec3 Diffuse = texture(uGbufferAlbedoSpec, tex_coord).rgb;
    float Specular = texture(uGbufferAlbedoSpec, tex_coord).a;
    
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse;// * 0.2; // hard-coded ambient component
    //vec3 viewDir  = normalize(viewPos - FragPos);



    
    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
   // if(draw_mode == 1)
        FragColor = vec4(lighting, 1.0) + vec4(0.4,0,0,1);
    //else if(draw_mode == 2)
    //    FragColor = vec4(FragPos, 1.0);
   // else if(draw_mode == 3)
    //    FragColor = vec4(Normal, 1.0);
    //else if(draw_mode == 4)
    //    FragColor = vec4(Diffuse, 1.0);
    //else if(draw_mode == 5)
    //	   FragColor = vec4(vec3(Specular), 1.0);
}