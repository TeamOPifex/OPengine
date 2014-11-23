varying lowp vec2 vUV;
varying lowp vec3 vTangent;
varying lowp vec3 vNormal;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;
uniform lowp vec2 uTexCoordScale;
uniform lowp vec2 uSpriteOffset;
uniform lowp vec4 uTint;

void main() {
	lowp vec3 biNormal = normalize(cross(vNormal, vTangent));
	lowp mat3 tanSpace = mat3(vTangent, biNormal, vNormal);
	lowp vec2 uv = vUV * uTexCoordScale + uSpriteOffset;
	lowp vec3 normal = normalize(tanSpace * (texture2D(uNormalTexture, uv) * 2.0 - 1.0).xyz);

	lowp float l = dot(normal, normalize(vec3(1, -1, -2)));
	lowp vec4 color = texture2D(uColorTexture, uv);

	gl_FragColor = color;
	gl_FragColor *= uTint;
}
