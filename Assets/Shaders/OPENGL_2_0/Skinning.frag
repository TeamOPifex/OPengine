uniform vec4 uColor;

attribute vec2 vTexCoord; 
attribute vec3 vNormal; 
attribute vec3 vTangent; 

attribute vec3 vPosition; 
attribute vec3 vEyeDirection; 
attribute vec3 vLightDirection; 

uniform sampler2D uColorTexture; 
uniform sampler2D uSpecularTexture; 
uniform sampler2D uNormalTexture; 

uniform vec3 uLightPosition; 
uniform vec2 uTextureScale; 

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	//vec4 tex = texture2D( uColorTexture, vTexCoord * uTextureScale );
	vec3 MaterialDiffuseColor =  vec4(1,0,0,1);
	vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor;
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = 
		vec4(
			MaterialAmbientColor +
			MaterialDiffuseColor * LightColor * LightPower * cosTheta, 
			1.0);
}
