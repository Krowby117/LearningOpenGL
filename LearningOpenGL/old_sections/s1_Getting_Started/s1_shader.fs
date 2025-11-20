// FRAGMENT SHADER FOR SECTION 1
#version 330 core
out vec4 FragColor;

in vec2 TexCord;

uniform float mixPerc;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, TexCord), texture(texture2, TexCord), 0.2f);
}
