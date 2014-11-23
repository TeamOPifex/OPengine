varying vec2 vUV; 

uniform sampler2D uColorTexture;
uniform sampler2D uOffsetTexture; 
uniform float uAlpha; 
uniform float uRadius;

void main() {
	vec2 uvp = (vUV * 2.0) - vec2(1.0);
	float p = uRadius * uRadius;
	float dist = dot(uvp, uvp);
	float delta = abs(dist - p) + 1.0;
	float a = max(0.0, (p/dist) - 1.0);
	
	
	vec2 uv = vec2(1.0) - vUV;
	vec2 offset  = (texture2D(uOffsetTexture, uv).xy - vec2(0.5)) / 4.0;
	gl_FragColor = texture2D(uColorTexture, uv + (offset * (1.0/delta)));
	
	gl_FragColor.a *= sqrt(a);
}
