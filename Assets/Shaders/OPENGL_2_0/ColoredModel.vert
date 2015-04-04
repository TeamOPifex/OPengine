attribute vec3 aPosition;
attribute vec3 aColor;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj;

varying vec3 vColor;

void main() {	
	
	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	gl_Position = (uProj * uView) * worldPos; 
	
	vColor = aColor;
}
