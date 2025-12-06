#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D outTexture;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0); // 틴트(색조) 조절용

void main()
{
    vec4 texColor = texture(outTexture, TexCoord);

    if(texColor.a < 0.1)
        discard;
        
    FragColor = texColor * color;
}