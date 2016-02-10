attribute lowp vec3 aPosition;
attribute lowp vec3 aColor;

uniform lowp mat4 uWorld;
uniform lowp mat4 uView;
uniform lowp mat4 uProj;

varying lowp vec3 vColor;

void main() {

	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	gl_Position = (uProj * uView) * worldPos;

	vColor = aColor;
}
