#version 330 core
out vec4 FragColor;

in vec4 vFragPos;
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uAlbedoMap;
uniform vec3 uLightPos;
uniform vec3 uCamPos;

void main()
{
	bool blinn = true;

    vec3 color = texture(uAlbedoMap, vUV).rgb;

    // Ambient
    vec3 ambient = 0.05 * color;

    // Diffuse
    vec3 lightDir = normalize(lightPos - vFragPos);
    vec3 normal = normalize(vNormal);
    float diff = max(dot(uLightPos, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
    vec3 viewDir = normalize(uCamPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}