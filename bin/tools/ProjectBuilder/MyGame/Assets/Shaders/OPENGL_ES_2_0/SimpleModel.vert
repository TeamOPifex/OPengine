attribute lowp vec3 aPosition;
attribute lowp vec2 aUV;
attribute lowp vec3 aNormal;
attribute lowp float aShadow;

uniform lowp mat4 uWorld; 
uniform lowp mat4 uView;
uniform lowp mat4 uProj; 
uniform lowp vec3 uLightPosition;

varying lowp vec2 vUV; 
varying lowp vec3 vNormal; 
varying lowp vec3 vEyeDirection; 
varying lowp vec3 vLightDirection; 
varying lowp float vShadow;

void main() {	
	
	lowp vec4 worldPos      = uWorld * vec4(aPosition, 1.0);
	lowp vec4 worldLightPos = vec4(0.0, 32.0, 0.0, 1.0);//uWorld * vec4(uLightPosition, 1.0);

	gl_Position = (uProj * uView) * worldPos; 
	
	lowp mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	lowp mat3 rotCam = mat3(uView[0].xyz, uView[1].xyz, uView[2].xyz);

	vEyeDirection = rotCam * vec3(0.0, 0.0, 1.0);
	vLightDirection = normalize(worldPos.xyz - worldLightPos.xyz);
	
	vNormal = rotScl * aNormal;
	vUV = aUV;
	vShadow = aShadow;
}
