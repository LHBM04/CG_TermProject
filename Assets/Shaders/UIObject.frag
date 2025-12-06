#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D outTexture;
uniform vec3 color = vec3(1.0, 1.0, 1.0); // 틴트(색조) 조절용

void main()
{
    vec4 texColor = texture(outTexture, TexCoord);
    
    // 투명한 부분(알파값 0.1 미만)은 그리지 않음
    if(texColor.a < 0.1)
        discard;
        
    // 조명 계산 없이 원본 색상 출력
    FragColor = texColor * vec4(color, 1.0);
}