#version 330 core

layout(location = 0) out vec4 FragColor;
uniform vec3 camera_pos = vec3(0.0f, 0.0f, 0.0f);
uniform float RADIUS = 0.5f;
uniform vec4 center = vec4(0.0f, 0.0f, -2.0f, 0.0f);
uniform mat4 view = mat4(1.0f);


bool collideWithSphere(float a, float b, float c);
float smoothClamp(float x, float a, float b);


void main()
{
    vec3 uv = (gl_FragCoord.xyz/768) * 2 - 1;
    // Always set the ray to be shot a z-distance of -1 from the camera
    uv.xy += camera_pos.xy;
    uv.z = camera_pos.z - 1.0f;

    vec3 ray = uv - camera_pos;
    ray = normalize(ray);

    vec3 sphere_center = (view * center).xyz;

    float a = dot(ray, ray);
    float b = 2.0f * dot(camera_pos, ray) - 2.0f * dot(sphere_center, ray);
    float c = dot(camera_pos, camera_pos) + dot(sphere_center, sphere_center) - RADIUS*RADIUS;

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
        vec3 hit_point = ray * t_hit;
        vec3 normal = (hit_point - sphere_center) / RADIUS;
        // Detect if normal vector is pointing up or down
        float brightness = dot(normal, vec3(0.0f, 1.0f, 0.0f));
        if(brightness >= 0)
        {
            FragColor = color;
        }
        else
        {
            FragColor = color * (1.0f - abs(brightness));
        }
    }

}

float smoothClamp(float x, float a, float b)
{
    float t = clamp(x, a, b);
    return t != x ? t : b + (a - b)/(1. + exp((b - a)*(2.*x - a - b)/((x - a)*(b - x))));
}

bool collideWithSphere(float a, float b, float c)
{
    
    // Quadratic formula thing
    // 
    float det = b*b - 4.0f*a*c;
    if(det < 0)
    {
        return false;
    }
    return true;
}
