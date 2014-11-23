
varying vec2 vUV; 
varying vec3 vNormal; 
varying vec3 vTangent;
varying vec3 vBullshit;

uniform sampler2D uColorTexture; 
uniform sampler2D uSpecularTexture; 
uniform sampler2D uNormalTexture; 

void main() {
	vec3 lightPos = vec3(1, -1, -2);
	vec3 lightDir = normalize(-lightPos);
	vec3 halfVec = normalize(vec3(0, 0, 1) + lightDir);
	vec3 biNormal = normalize(cross(vNormal, vTangent));
	mat3 tanSpace = mat3(vTangent, biNormal, vNormal);
	vec3 normal = normalize(tanSpace * vBullshit);
	vec4 color = texture2D(uColorTexture, vUV);
	vec4 diffuse = vec4(vec3(dot(lightDir, normal)), 1);
	vec4 specular = texture2D(uSpecularTexture, vUV) * pow(dot(halfVec, normal), 64.0);
	gl_FragColor = vec4((color.xyz * diffuse.xyz) + specular.xyz, 1);
}
