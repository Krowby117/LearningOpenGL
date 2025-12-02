#version 330 core
// VERTEX SHADER
layout (location = 0) in vec2 aPos;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(aPos, 0.0, 1.0);
}
