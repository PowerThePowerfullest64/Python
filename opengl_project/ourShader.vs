#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0

out vec3 position;

uniform float move;

void main()
{
    gl_Position = vec4(aPos.x + move, aPos.yz, 1.0);
    position = aPos + abs(move);
}