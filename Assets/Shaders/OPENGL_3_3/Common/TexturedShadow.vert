#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

// Values that stay constant for the whole mesh.
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uViewShadow;
uniform mat4 uProjShadow;

void main(){

	// Output position of the vertex, in clip space : uMVP * position
	gl_Position = uProj * uView * uWorld * vec4(aPosition, 1.0);	
	
    vs_out.FragPos = vec3(uWorld * vec4(aPosition, 1.0));
    vs_out.Normal = transpose(inverse(mat3(uWorld))) * aNormal;
    vs_out.TexCoords = aUV;
    vs_out.FragPosLightSpace = uProjShadow * uViewShadow * vec4(vs_out.FragPos, 1.0);
}
