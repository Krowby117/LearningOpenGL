// FRAGMENT SHADER FOR SECTION 1
#version 330 core
out vec4 FragColor;

in vec2 TexCord;

// texture samplers
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCord);
}
