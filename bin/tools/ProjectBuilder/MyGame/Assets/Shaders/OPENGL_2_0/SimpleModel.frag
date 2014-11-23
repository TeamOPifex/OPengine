varying vec3 vNormal; 
varying vec2 vUV;
varying vec3 vLightDirection;
varying float vShadow;

uniform vec3 uColor;
uniform sampler2D uColorTexture;

void main() {
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.2;
	
	vec2 uv = vUV;
	if(uv.x <= 0.0) uv.x = 0.0001;
	if(uv.x >= 1.0) uv.x = 0.9999;
	if(uv.y <= 0.0) uv.y = 0.0001;
	if(uv.y >= 1.0) uv.y = 0.9999;

	float shadow = ceil(vShadow * 32.0) / 32.0;
	vec3 MaterialColor = texture2D(uColorTexture, uv).rgb;
	vec3 MaterialAmbientColor = MaterialColor * 0.2;
	vec3 MaterialDiffuseColor = MaterialColor * 0.8 * shadow;

	vec3 n = normalize( vNormal );
	vec3 l = normalize( vec3( 0.0, -1.0, 0.0 ) );
	float d = -dot( n, l );
	float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = vec4(
		MaterialAmbientColor + MaterialDiffuseColor, 
		1.0
	);
}
