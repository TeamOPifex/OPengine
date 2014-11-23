attribute lowp vec3 aPosition;
attribute lowp vec3 aNormal;
attribute lowp vec2 aUV;

uniform lowp mat4 uWorld; 
uniform lowp mat4 uView;
uniform lowp mat4 uProj; 

varying lowp vec2 vUV; 
varying lowp vec3 vNormal;
varying lowp vec3 vTangent; 

void main() {
	lowp vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV; 
	
	lowp mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	vNormal = normalize(rotScl * aNormal);
	vTangent = normalize(rotScl * vec3(1, 0, 0));
}
