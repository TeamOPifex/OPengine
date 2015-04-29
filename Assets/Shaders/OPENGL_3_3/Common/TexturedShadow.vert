#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

// Output data ; will be interpolated for each fragment.
out vec2 vUV;
out vec3 vNormal;
out vec4 vShadowCoord;
out vec3 vLightDirection;

// Values that stay constant for the whole mesh.
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uViewShadow;
uniform mat4 uProjShadow;
uniform mat4 uBias;
uniform vec3 uLightDirection;

void main(){

	// Output position of the vertex, in clip space : uMVP * position
	gl_Position = uProj * uView * uWorld * vec4(aPosition,1);
	
	vShadowCoord = uBias * uProjShadow * uViewShadow * uWorld * vec4(aPosition,1);
	
	// UV of the vertex. No special space for this one.
	vUV = aUV;

	vLightDirection = (uView * vec4(uLightDirection,0)).xyz;

	vNormal = ( uView * uWorld * vec4(aNormal,0)).xyz;
}
