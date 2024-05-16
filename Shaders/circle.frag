#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

uniform float RADIUS;
uniform vec3 center;

vec2 uv = (gl_FragCoord.xy/768 * 2 - 1) - center.xy;
void main()
{
    if(length(uv) > RADIUS)
    {
        discard;
    }
    else
    {
        FragColor = vec4(vertexColor);
    }
}
