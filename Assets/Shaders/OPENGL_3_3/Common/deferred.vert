#version 330 core

in vec3 aPosition;
in vec3 aNormal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;
               
out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

void main()
{       
    gl_Position    = uProj * uView * uWorld * vec4(aPosition, 1.0);
    vNormal        = (uWorld * vec4(aNormal, 0.0)).xyz;
    vWorldPos      = (uWorld * vec4(aPosition, 1.0)).xyz;
}