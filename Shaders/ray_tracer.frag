#version 330 core

#define BRIGHTNESS_COEFF 0.7
#define M_PI 3.141592653589

layout(location = 0) out vec4 FragColor;

layout (std140) uniform matrices
{
    mat4 view;
    mat4 rotation;
    vec3 sky;
    float fovy;
};

struct Ray
{
    vec3 origin;
    vec3 dir;
};

struct Sphere
{
    vec3 center;
    float radius;
};

float smoothClamp(float x, float a, float b);
float rand(vec2 co);
vec3 reflect(vec3 ray, vec3 normal);
vec3 radiance(Ray ray);
bool intersectSphere(Ray ray, Sphere sphere, out vec3 t_hit, out vec3 normal);

void main()
{
    Ray ray;
    ray.origin = vec3(0.0f);
    ray.dir.xyz = (gl_FragCoord.xyz/768.0f) * 2.0f - 1.0f;
    // Divide by value of 90/fovy
    ray.dir.xy *= tan((M_PI/180) * (fovy/2));
    ray.dir.z = -1.0f;

    Sphere sphere;
    vec4 center = vec4(0.0f, 0.0f, -2.0f, 1.0f);
    sphere.center = (rotation * view * center).xyz;
    sphere.radius = 0.5f;

    float a = dot(ray.dir, ray.dir);
    float b = - 2.0f * dot(sphere.center, ray.dir);
    float c = dot(sphere.center, sphere.center) - sphere.radius*sphere.radius;

    float det = b*b - 4.0f*a*c;

    if(det < 0)
    {
        // vec4 Light_Gray = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0)
        if(ray.dir.z < 0)
        {
            FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0f);
        }
        else
        {
            FragColor = vec4(80.0f/255, 110.0f/255, 173.0f/255, 1.0f);
        }
    }
    else
    {
        float t_hit = (-b - sqrt(det)) / (2*a);
        vec3 hit_point = ray.origin + ray.dir * t_hit;
        vec3 normal = normalize(hit_point - sphere.center);
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

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 reflect(vec3 ray, vec3 normal)
{
    return ray + 2*dot(normal, ray) * normal;
}

vec3 radiance(Ray ray)
{
    // If ray intersects with circle
    // Calculate attenuation based on Lambertian BRDF
    // or use skyColor(ray);
    return vec3(1.0f, 1.0f, 1.0f);
}

/*
 *  Should take in unit vector
 */
vec3 skyColor(Ray ray)
{
    float t = 0.5 * (ray.dir.y + 1.0f);
    return (1.0 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}
