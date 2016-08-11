#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBinormal;
layout(location = 4) in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 uCamPos;
uniform mat4 uDepthBiasMVP;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vUV;
out vec3 vBinormal;
out vec3 vTangent;
out vec3 vColor;
out vec4 vShadowCoord;
out vec3 vCamPos;

void main()
{
	vec4 pos = uWorld * vec4(aPosition, 1);
	vPosition = pos;

	gl_Position = uProj * uView * pos;

	mat3 model = mat3(uWorld);

	vNormal = model * aNormal;
	vBinormal = model * aBinormal;
	vTangent = model * aTangent;
	vUV = vec2(aUV.x, 1.0 - aUV.y);
	//vUV = aUV;
	vColor = vec3(1.0);
	vCamPos = uCamPos;

	vShadowCoord = uDepthBiasMVP * pos;
}
