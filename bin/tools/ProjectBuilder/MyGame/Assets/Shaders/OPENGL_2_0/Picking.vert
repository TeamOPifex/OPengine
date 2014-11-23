attribute vec3 aPosition;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec4 uColor;

varying vec4 vColor;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	vColor = uColor;	
}
