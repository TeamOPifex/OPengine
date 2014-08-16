varying vec2 vUV;
varying vec3 vTangent;
varying vec3 vNormal;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;
uniform vec2 uTexCoordScale;
uniform vec2 uSpriteOffset;
uniform vec4 uTint;

void main() {
	vec3 biNormal = normalize(cross(vNormal, vTangent));
	mat3 tanSpace = mat3(vTangent, biNormal, vNormal);
	vec2 uv = vUV * uTexCoordScale + uSpriteOffset;
	vec3 normal = normalize(tanSpace * (texture2D(uNormalTexture, uv) * 2.0 - 1.0).xyz);

	float l = dot(normal, normalize(vec3(1, -1, -2)));
	vec4 color = texture2D(uColorTexture, uv);

	gl_FragColor = color;
	gl_FragColor *= uTint;
}
