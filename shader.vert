#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 color;

smooth out vec4 vertColor;

uniform mat4 gRotation;

void main()
{
    gl_Position = gRotation * vec4(Position, 1.0);
    vertColor = color;
}
