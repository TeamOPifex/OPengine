#version 330 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vWorldPos;

uniform sampler2D uColorTexture;

void main()									
{											
	gl_FragData[0] = vec4(vWorldPos, 1);
	gl_FragData[1] = vec4(1,1,1,1);
	gl_FragData[2] = vec4(normalize(vNormal), 1);
}
