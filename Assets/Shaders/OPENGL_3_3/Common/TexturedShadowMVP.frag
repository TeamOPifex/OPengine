#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D uColorTexture;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

out vec4 FragColor;

void main()
{           
    vec3 color = texture(uColorTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);

    vec3 lightColor = vec3(0.4);

    // Ambient
    vec3 ambient = 0.4 * color;


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
    vec3 specular = vec3(0,0,0);//spec * lightColor;    

    // Calculate shadow
	// float shadow = 0;             
    vec3 lighting = (ambient + (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0f);
}