attribute vec3 aPosition;
attribute vec4 aColor;

uniform mat4 uProj;

varying vec4 vColor;

void main() {
	gl_Position = uProj * vec4(aPosition,1);
	
	vColor = aColor; 
}
