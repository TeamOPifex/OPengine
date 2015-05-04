#version 330 core

uniform vec4 uColor;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;

in vec3 vPosition;
in vec3 vEyeDirection;
in vec3 vLightDirection;

uniform sampler2D uColorTexture; 
uniform sampler2D uSpecularTexture; 
uniform sampler2D uNormalTexture; 

uniform vec3 uLightPosition; 
uniform vec2 uTextureScale;

out vec4 FragColor;

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	vec2 v = vec2(vTexCoord.x, 1.0 - vTexCoord.y);
	vec3 MaterialAmbientColor = texture(uColorTexture, v).rgb + vec3(0.2);
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	FragColor =
		vec4(
			MaterialAmbientColor +
			MaterialAmbientColor * LightColor * LightPower * cosTheta, 
			1.0);
}
