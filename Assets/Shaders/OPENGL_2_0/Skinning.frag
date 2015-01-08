uniform vec4 uColor;

varying vec2 vTexCoord; 
varying vec3 vNormal; 
varying vec3 vTangent; 

varying vec3 vPosition; 
varying vec3 vEyeDirection; 
varying vec3 vLightDirection; 

uniform sampler2D uColorTexture; 
uniform sampler2D uSpecularTexture; 
uniform sampler2D uNormalTexture; 

uniform vec3 uLightPosition; 
uniform vec2 uTextureScale; 

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	vec2 v = vec2(vTexCoord.x, 1.0 - vTexCoord.y);
	vec3 MaterialAmbientColor = texture2D(uColorTexture, v).rgb + vec3(0.2);
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = 
		vec4(
			MaterialAmbientColor +
			MaterialAmbientColor * LightColor * LightPower * cosTheta, 
			1.0);
}
