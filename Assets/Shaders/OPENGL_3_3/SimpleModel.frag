#version 330 core

in vec3 vNormal;
in vec3 vLightDirection;

out vec4 FragColor;

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	vec3 MaterialDiffuseColor =  vec3(1,0,0);
	vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor;
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	FragColor =
		vec4(
			MaterialAmbientColor +
			MaterialDiffuseColor * LightColor * LightPower * cosTheta, 
			1.0);
}
