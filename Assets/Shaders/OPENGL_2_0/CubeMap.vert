attribute vec3 aPosition;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

varying vec3 vUV;

void main()
{
    gl_Position = uProj * uView * uWorld * vec4(aPosition, 1.0);
    vUV = normalize(aPosition);
}