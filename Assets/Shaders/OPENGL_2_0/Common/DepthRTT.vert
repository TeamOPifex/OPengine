// Input vertex data, different for all executions of this shader.
attribute vec3 aPosition;

// Values that stay constant for the whole mesh.
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main(){
	gl_Position =  uProj * uView * uWorld * vec4(aPosition,1);
}

