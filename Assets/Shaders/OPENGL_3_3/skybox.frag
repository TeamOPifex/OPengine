#version 330 core
out vec4 FragColor;

in vec3 vPos;
  
uniform samplerCube uEnvironmentMap;
  
void main()
{
	vec3 envColor = textureLod(uEnvironmentMap, vPos, 1.2).rgb; 
    //vec3 envColor = texture(uEnvironmentMap, vPos).rgb;
    
	// Gamma correction
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
}