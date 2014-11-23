attribute vec4 vColor;
attribute vec3 vNormal; 
attribute vec3 vLightDirection; 
attribute float vLightPower; 

void main() {
	vec3 LightColor = vec3(1,1,1);
	
	vec3 MaterialDiffuseColor = vColor.rgb;
	vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor;
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = 
		vec4(
			MaterialAmbientColor +
			MaterialDiffuseColor * LightColor * vLightPower * cosTheta, 
			1.0);
}
