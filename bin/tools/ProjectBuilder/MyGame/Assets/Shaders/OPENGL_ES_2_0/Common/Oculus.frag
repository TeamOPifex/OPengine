#version 330

out vec4 outputColor;

uniform sampler2D texture0;

in vec2 oTexCoord0;
in vec2 oTexCoord1;
in vec2 oTexCoord2;
in float oVignette;

void main() {

    float r = texture(texture0, oTexCoord0).r;
    float g = texture(texture0, oTexCoord1).g;
    float b = texture(texture0, oTexCoord2).b;

    outputColor = vec4(r, g, b, 1) * oVignette;

    //outputColor = vec4(1, 1, 0, 1);
}