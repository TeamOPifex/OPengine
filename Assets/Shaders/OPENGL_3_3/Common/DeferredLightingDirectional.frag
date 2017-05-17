#version 330 core

out vec4 FragColor;
in vec3 vPos;

uniform sampler2D uGbufferPosition;
uniform sampler2D uGbufferNormal;
uniform sampler2D uGbufferAlbedoSpec;

//uniform vec3 viewPos;


void main()
{             

	vec2 tex_coord = gl_FragCoord.xy/vec2(1920, 1080);

    // Retrieve data from gbuffer
    vec3 FragPos = texture(uGbufferPosition, tex_coord).rgb;
    vec3 Diffuse = texture(uGbufferAlbedoSpec, tex_coord).rgb;
    vec3 Normal = texture(uGbufferNormal, tex_coord).rgb;
    float Specular = texture(uGbufferAlbedoSpec, tex_coord).a;
    
	vec3 lighting  = Diffuse * 0.1;
	vec3 lightPos = vec3(0,3,0);
	vec3 lightDir = vec3(0,1,0);
	float distance = length(lightPos - FragPos);
	if(distance < 5.0) {
        // Diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * vec3(1,1,1);
        // Specular
        //vec3 halfwayDir = normalize(lightDir + viewDir);  
        //float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        //vec3 specular = lights[i].Color * spec * Specular;
        // Attenuation
        float attenuation = 1.0 / (1.0 + 1.0 * distance * distance);
        diffuse *= attenuation;
       // specular *= attenuation;
        lighting += diffuse;// + specular;
	}
    
    // Based on which of the 1-5 keys we pressed, show final result or intermediate g-buffer textures
   // if(draw_mode == 1)
        FragColor = vec4(lighting, 1.0);// + vec4(0.4,0,0,1);
    //else if(draw_mode == 2)
    //    FragColor = vec4(FragPos, 1.0);
   // else if(draw_mode == 3)
    //    FragColor = vec4(Normal, 1.0);
    //else if(draw_mode == 4)
    //    FragColor = vec4(Diffuse, 1.0);
    //else if(draw_mode == 5)
    //	   FragColor = vec4(vec3(Specular), 1.0);
}