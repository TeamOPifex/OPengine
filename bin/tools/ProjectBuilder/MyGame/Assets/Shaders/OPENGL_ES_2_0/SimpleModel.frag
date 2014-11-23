varying lowp vec3 vNormal; 
varying lowp vec2 vUV;
varying lowp vec3 vLightDirection;
varying lowp float vShadow;

uniform lowp vec3 uColor;
uniform sampler2D uColorTexture;

void main() {
	lowp vec3 LightColor = vec3(1,1,1);
	lowp float LightPower = 0.2;
	
	lowp float shadow = ceil(vShadow * 32.0) / 32.0;
	lowp vec3 MaterialColor = texture2D(uColorTexture, vUV).rgb * shadow;
	lowp vec3 MaterialAmbientColor = MaterialColor * 0.8;
	lowp vec3 MaterialDiffuseColor = MaterialColor * 0.6;

	lowp vec3 n = normalize( vNormal );
	lowp vec3 l = normalize(vec3(0.0, -1.0, 0.0));
	lowp float d = -dot( n, l );
	lowp float cosTheta = clamp( d, 0.0, 1.0 );
	
	gl_FragColor = vec4(
		MaterialDiffuseColor * d + MaterialAmbientColor, 
		1.0
	);
}
