#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

uniform vec3 center;

void main()
{
    vec3 new_center = aPos + center;
    float red   = dot(new_center, vec3(0.0, 1.0, 0.0));
    float green = dot(new_center, vec3(1.0, -1.0, 0.0));
    float blue  = dot(new_center, vec3(-1.0, -1.0, 0.0));

    gl_Position = vec4(new_center, 1.0);
    vertexColor = vec4(red, green, blue, 1.0);
}
