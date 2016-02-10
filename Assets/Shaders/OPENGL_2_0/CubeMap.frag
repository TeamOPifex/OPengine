varying vec3 vUV;

uniform samplerCube uColorTexture;

void main()
{
    gl_FragColor = textureCube(uColorTexture, vUV);
}