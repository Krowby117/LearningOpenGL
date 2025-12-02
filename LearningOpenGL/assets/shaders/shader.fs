#version 330 core
// FRAGMENT SHADER
out vec4 FragColor;

uniform vec3 fragCol;

void main()
{    
    FragColor = vec4(fragCol, 1.0f);
}
