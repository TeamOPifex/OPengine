varying vec2 vUV;
varying vec3 vNormal;
varying vec3 vWorldPos;

uniform sampler2D uColorTexture;

void main()									
{											
	gl_FragData[0] = vec4(vWorldPos, 1);					
	gl_FragData[1] = vec4(1,1,1,1);
	gl_FragData[2] = vec4(normalize(vNormal), 1);
}
