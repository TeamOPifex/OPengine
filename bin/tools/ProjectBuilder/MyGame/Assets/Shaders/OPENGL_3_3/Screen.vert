attribute vec3 aPosition; uniform mat4 uWorld;void main() {	vec4 screenPos = uWorld * vec4(aPosition,1);	gl_Position = screenPos;}
