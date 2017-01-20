#version 330 core

in vec2 vUV;
out float FragColor;

uniform sampler2D uSSAOBuffer;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(uSSAOBuffer, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(uSSAOBuffer, vUV + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
	//FragColor = texture(uSSAOBuffer, vUV).rgb;
}  