#version 330 core

#define BRIGHTNESS_COEFF 0.7

layout(location = 0) out vec4 FragColor;
uniform vec3 camera_pos = vec3(0.0f, 0.0f, 0.0f);
uniform float RADIUS = 0.5f;
uniform vec4 center = vec4(0.0f, 0.0f, -2.0f, 1.0f);
uniform mat4 rotation = mat4(1.0f);


float smoothClamp(float x, float a, float b);
float rand(vec2 co);

void main()
{
    vec3 uv = (gl_FragCoord.xyz/768) * 2 - 1;
    uv.xy *= 0.5;
    // Always set the ray to be shot a z-distance of -1 from the camera
    uv.z = -1.0f;

    vec3 ray = uv;
    vec3 sphere_center = (rotation * center).xyz;

    float a = dot(ray, ray);
    float b = 2.0f * dot(camera_pos, ray) - 2.0f * dot(sphere_center, ray);
    float c = dot(camera_pos, camera_pos) + dot(sphere_center, sphere_center)
                - 2.0f * dot(camera_pos, sphere_center) - RADIUS*RADIUS;

    float det = b*b - 4.0f*a*c;

    if(det < 0)
    {
        // No hit so we set to background color Light Gray
        // vec4 Light_Gray = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0)
        FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);
    }
    else
    {
        // Color the pixel as if there was skybox lighting from above
        // Using the normal vector
        vec4 color = vec4(0.0f, 0.5f, 0.5f, 1.0f);

        float t_hit = (-b - sqrt(det)) / (2*a);
        vec3 hit_point = camera_pos + (ray * t_hit);
        vec3 normal = normalize(hit_point - sphere_center);
        // Detect if normal vector is pointing up or down
        vec4 sky = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        float brightness = max(dot(normal, sky.xyz), 0);

        if(t_hit > 0)
        {
            FragColor = vec4(normal * 0.5f + 0.5f, 1.0f) * brightness;
        }
        else{
            // Background color Light Gray
            FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);

            // I think if FragColor is never set, OpenGL or maybe the graphics driver instead does
            // some weird thing where it just guesses what you wanted and uses code somewhere else
            // to pick a color because when I set FragColor in the else statement and the sphere
            // is behind me, it still shows up in front of me just flipped uptside down, so my
            // only explanation is that somehow it's like using the code in the if statement before
            // it although I don't know how it chooses what to render there, though.
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
