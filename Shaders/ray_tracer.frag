#version 330 core

#define BRIGHTNESS_COEFF 0.7
#define M_PI 3.141592653589

layout(location = 0) out vec4 FragColor;

uniform float RADIUS = 1.0f;
uniform vec4 center = vec4(0.0f, 0.0f, -2.0f, 1.0f);

// Light Brown #9e5341

layout (std140) uniform matrices
{
    mat4 view;
    mat4 rotation;
    float fovy;
};


float smoothClamp(float x, float a, float b);
float rand(vec2 co);

void main()
{
    vec3 uv = (gl_FragCoord.xyz/768.0f) * 2.0f - 1.0f;
    // Divide by value of 90/fovy
    uv.xy *= tan(M_PI * (fovy/2 / 180));
    // Always set the ray to be shot a z-dist of -1 from the camera
    uv.z = -1.0f;

    vec3 ray = uv;
    vec3 sphere_center = (view * rotation * center).xyz;

    float a = dot(ray, ray);
    float b = - 2.0f * dot(sphere_center, ray);
    float c = dot(sphere_center, sphere_center) - RADIUS*RADIUS;

    float det = b*b - 4.0f*a*c;

    if(det < 0)
    {
        // vec4 Light_Gray = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0)
        FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);
    }
    else
    {
        float t_hit = (-b - sqrt(det)) / (2*a);
        vec3 hit_point = ray * t_hit;
        vec3 normal = normalize(hit_point - sphere_center);
        vec3 sky = vec3(0.0f, 1.0f, 0.0f);
        float brightness = max(dot(normal, sky), 0);

        if(t_hit > 0)
        {
            FragColor = vec4(normal * 0.5f + 0.5f, 1.0f) * brightness;
        }
        else
        {
            FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);
        }
    }

}

float smoothClamp(float x, float a, float b)
{
    float t = clamp(x, a, b);
    return t != x ? t : b + (a - b)/(1. + exp((b - a)*(2.*x - a - b)/((x - a)*(b - x))));
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
