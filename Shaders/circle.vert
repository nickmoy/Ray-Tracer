#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

uniform vec3 center;
uniform vec4 color;

void main()
{
    vec3 new_center = aPos + center;
    
    vec4 background = vec4(210.0/255, 222.0/255, 228.0/255, 1.0);

    gl_Position = vec4(new_center, 1.0);
    vertexColor = color;
}
