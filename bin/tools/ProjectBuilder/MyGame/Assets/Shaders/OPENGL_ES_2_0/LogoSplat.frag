varying lowp vec2 vUV; 

uniform sampler2D uColorTexture;
uniform sampler2D uOffsetTexture; 
uniform lowp float uAlpha; 
uniform lowp float uRadius;

void main() {
	lowp vec2 uvp = (vUV * 2.0) - vec2(1.0);
	lowp float p = uRadius * uRadius;
	lowp float dist = dot(uvp, uvp);
	lowp float delta = abs(dist - p) + 1.0;
	lowp float a = max(0.0, (p/dist) - 1.0);
	
	
	lowp vec2 uv = vec2(1.0) - vUV;
	lowp vec2 offset  = (texture2D(uOffsetTexture, uv).xy - vec2(0.5)) / 4.0;
	gl_FragColor = texture2D(uColorTexture, uv + (offset * (1.0/delta)));
	
	gl_FragColor.a *= sqrt(a);
}
