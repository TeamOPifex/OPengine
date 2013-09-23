#version 330 core

in vec2 TexCoordOut; 
in vec3 NormalOut; 
in vec3 TangentOut; 

uniform sampler2D uColorTexture; 
uniform sampler2D uSpecularTexture; 
uniform sampler2D uNormalTexture; 

void main() {
	vec3 lightPos = vec3(1, -1, -2);
	vec3 lightDir = normalize(-lightPos);
	vec3 halfVec = normalize(vec3(0, 0, 1) + lightDir);
	vec3 biNormal = normalize(cross(NormalOut, TangentOut));
	mat3 tanSpace = mat3(TangentOut, biNormal, NormalOut);
	vec3 normal = normalize(tanSpace * (texture2D(uNormalTexture, TexCoordOut) * 2.0 - 1.0).xyz);
	vec4 color = texture2D(uColorTexture, TexCoordOut);
	vec4 diffuse = vec4(vec3(dot(lightDir, normal)), 1);
	vec4 specular = texture2D(uSpecularTexture, TexCoordOut) * pow(dot(halfVec, normal), 64.0);
	gl_FragColor = vec4(((normal / 2.0) + 0.5), 1);//vec4((color.xyz * diffuse.xyz) + specular.xyz, 1);
}
