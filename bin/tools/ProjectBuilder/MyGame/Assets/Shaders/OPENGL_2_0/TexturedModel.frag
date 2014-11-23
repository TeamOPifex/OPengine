attribute vec3 vNormal; 
attribute vec2 vUV; 
attribute vec3 vLightDirection;

uniform sampler2D uColorTexture; 

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	vec3 MaterialDiffuseColor =  vec4(1,1,1,1);
	vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor * texture2D(uColorTexture, vUV).rgb;
	
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
