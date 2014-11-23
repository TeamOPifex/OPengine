varying vec3 vNormal; 
varying vec2 vUV; 
varying vec3 vLightDirection;

uniform sampler2D uColorTexture;

void main() {
	float LightPower = 0.2;
	vec3 ambient = vec3(0.2, 0.2, 0.2);
	vec3 diffuse = texture2D(uColorTexture, vUV).rgb;

	vec3 n = normalize( vNormal );
	vec3 l = normalize( vLightDirection );
	float d = dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );

	gl_FragColor = 
		vec4(
			diffuse + cosTheta * 0.1, 
			1.0);
}
