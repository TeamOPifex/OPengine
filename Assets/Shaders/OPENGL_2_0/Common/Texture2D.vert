attribute vec3 aPosition;
attribute vec2 aUV;
varying vec2 vUV;

uniform mat4 uWorld;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;

	vUV = vec2(aUV.x, 1.0 - aUV.y);
}
