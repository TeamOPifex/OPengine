varying lowp vec3 vNormal; 
varying lowp vec3 vLightDirection;

void main() {
	lowp vec3 LightColor = vec3(1,1,1);
	lowp float LightPower = 0.2;
	
	lowp vec3 MaterialDiffuseColor = vec3(1,0,0);
	lowp vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor;
	
	lowp vec3 n = normalize( vNormal );
	lowp vec3 l = normalize( vLightDirection );
	lowp float d = dot( n, l );
	lowp float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = 
		vec4(
			MaterialAmbientColor +
			MaterialDiffuseColor * LightColor * LightPower * cosTheta, 
			1.0);
}
