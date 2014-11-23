attribute vec3 vColor;
attribute vec3 vNormal; 
attribute vec3 vLightDirection; 
attribute vec3 vEyePosition; 
attribute vec3 vAmbient; 
attribute vec3 vWorldSpace; 
attribute vec3 vLightPos; 
attribute float vLightPower; 

void main() {
	
	vec3 LightColor = vec4(1,1,1,1);
	
	vec3 MaterialDiffuseColor = vColor;
	vec3 MaterialAmbientColor = vAmbient * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	
	float distance = length( vLightPos - vWorldSpace );
	
	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	
	gl_FragColor =
		vec4( MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * vLightPower * cosTheta  / (distance*distance) , 1.0);// * (),  1.0);
		
}
