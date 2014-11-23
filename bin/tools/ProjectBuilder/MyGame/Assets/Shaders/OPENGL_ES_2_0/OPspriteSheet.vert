attribute lowp vec3 aPosition;
attribute lowp vec2 aUV;

uniform lowp mat4 uWorld;
uniform lowp vec2 uOffset;
uniform lowp vec2 uSize;

varying lowp vec2 vUV;

void main() {
	lowp vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;
	
	vUV = (aUV * vec2(1.0, -1.0) + vec2(0.0,1.0)) * uSize + uOffset; 
}
