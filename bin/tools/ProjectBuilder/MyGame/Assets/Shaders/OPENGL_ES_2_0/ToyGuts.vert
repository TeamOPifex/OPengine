attribute lowp vec3  aPosition; // 12 bytes
attribute lowp vec2  aUV;       // 8 bytes
attribute lowp vec3  aVelocity; // 12 bytes
attribute lowp float aBirth;    // 4 bytes

uniform lowp float uTime;
uniform lowp mat4 uView;
uniform lowp mat4 uProj;

varying lowp vec2 vUV;
varying lowp vec2 vUvSize;

void main(void)
{
	lowp vec4 worldPosition = vec4(aPosition, 1.0);

	gl_Position = uProj * uView * worldPosition;
	vUV = aUV;
}