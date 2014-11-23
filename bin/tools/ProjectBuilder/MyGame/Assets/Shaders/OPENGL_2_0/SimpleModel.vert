attribute vec3 aPosition;
attribute vec2 aUV;
attribute vec3 aNormal;
attribute float aShadow;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 
uniform vec3 uLightPosition;

varying vec2 vUV; 
varying vec3 vNormal; 
varying vec3 vEyeDirection; 
varying vec3 vLightDirection; 
varying float vShadow;

void main() {	
	
	vec4 worldPos      = uWorld * vec4(aPosition, 1.0);
	vec4 worldLightPos = vec4(0.0, 32.0, 0.0, 1.0);//uWorld * vec4(uLightPosition, 1.0);

	gl_Position = (uProj * uView) * worldPos; 
	
	mat3 rotScl = mat3(uWorld[0].xyz, uWorld[1].xyz, uWorld[2].xyz); 
	mat3 rotCam = mat3(uView[0].xyz, uView[1].xyz, uView[2].xyz);

	vEyeDirection = rotCam * vec3(0.0, 0.0, 1.0);
	vLightDirection = normalize(worldPos.xyz - worldLightPos.xyz);
	
	vNormal = rotScl * aNormal;
	vUV = aUV;
	vShadow = aShadow;
}
