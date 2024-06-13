#version 330 core

#define M_PI 3.141592653589
#define NUM_OBJS 3
#define NUM_BOUNCES 2
#define NUM_SAMPLES 1
#define T_MIN -0.01f
#define T_MAX 1000.0f

const int LAMBERTIAN = 0;
const int LIGHT = 1;
const int DIELECTRIC = 2;

layout(location = 0) out vec4 FragColor;

layout (std140) uniform matrices
{
    mat4 view;
    mat4 rotation;
    vec3 sky;
    float fovy;
};

struct Material
{
    int type;
    vec3 albedo;
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
    Material material;
};

const Sphere m_objects[NUM_OBJS] = Sphere[]
(
    Sphere(vec3(0.7f, 0.5f, -2.0f), 0.5f, Material(1, vec3(0.85f))),
    Sphere(vec3(-0.7f, 0.3f, -2.0f), 0.3f, Material(0, vec3(186.0f/255, 121.0f/255, 86.0f/255))),
    Sphere(vec3(0.0f, -100.0f, -2.0f), 100.0f, Material(0, vec3(80.0f/420, 110.0f/420, 173.0f/420)))
);


float smoothClamp(float x, float a, float b);
float rand(vec2 co);
vec3 reflect(Ray ray, vec3 normal);
void radiance(Ray ray, out Ray ray_next, inout vec3 attenuation, inout bool stop);
bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal);
vec3 skyColor(Ray ray);
vec3 randUnitVector(vec3 normal);
vec3 randVectorInHemisphere(vec3 normal);
vec3 shootRay(Ray ray);


void main()
{
    // Ray from camera through pixel
    Ray ray;
    ray.origin = vec3(0.0f);
    ray.dir.xyz = (gl_FragCoord.xyz/768.0f) * 2.0f - 1.0f;
    ray.dir.xy *= tan((M_PI/180) * (fovy/2));
    ray.dir.z = -1.0f;

    vec3 color = vec3(0.0f);
    for(int i = 0; i < NUM_SAMPLES; i++)
    {
        color += shootRay(ray);
    }
    color /= NUM_SAMPLES;

    FragColor = vec4(color, 1.0f);
}

vec3 shootRay(Ray ray)
{
    Ray ray_next;
    vec3 color = vec3(1.0f);
    vec3 attenuation = vec3(1.0f);
    bool stop = false;
    for(int i = 0; i < NUM_BOUNCES; i++)
    {
        radiance(ray, ray_next, attenuation, stop);
        color *= attenuation;
        ray = ray_next;
        if(stop)
        {
            break;
        }
    }
    return color;
}

/*
 *  Shoot ray in scene and calculate closest intersection by looping through spheres
 */
void radiance(Ray ray, out Ray ray_next, inout vec3 attenuation, inout bool stop)
{
    // If ray intersects with circle
    // Calculate attenuation based on Lambertian BRDF
    // or use skyColor(ray);
    
    float closest_hit = 2000.0f;
    for(int i = 0; i < NUM_OBJS; i++)
    {
        Sphere sphere = m_objects[i];
        sphere.center = (rotation * view * vec4(sphere.center, 1.0f)).xyz;

        float t_hit;
        vec3 hit_point;
        vec3 normal;

        bool did_hit = intersectSphere(ray, sphere, t_hit, hit_point, normal);

        if(did_hit && t_hit < closest_hit && t_hit > T_MIN && t_hit < T_MAX)
        {
            closest_hit = t_hit;
            ray_next.origin = hit_point;
            ray_next.dir = reflect(ray, normal);
            if(sphere.material.type == 0)
            {
                float brightness = max(dot(normal, sky), 0);
                // attenuation = sphere.material.albedo * (normal * 0.5f + 0.5f) * brightness;
                attenuation = sphere.material.albedo * brightness;
                stop = true;
            }
            else if(sphere.material.type == 1)
            {
                // attenuation = vec3(1.0f);
                attenuation = sphere.material.albedo;
            }
        }
    }
    
    if(closest_hit >= 1000.0f)
    {
        stop = true;
        ray_next.origin = vec3(0.0f);
        ray_next.dir = vec3(0.0f, 1.0f, 0.0f);
        attenuation = skyColor(ray);

        // if(dot(ray.dir.xyz, sky) > 0)
        // {
        //     attenuation = skyColor(ray);
        //     ray_next.dir = vec3(0.0f, 1.0f, 0.0f);
        // }
        // else
        // {
        //     attenuation = vec3(80.0f/255, 110.0f/255, 173.0f/255);
        //     ray_next.dir = vec3(0.0f, -1.0f, 0.0f);
        // }

    }
}

bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal)
{
    float a = dot(ray.dir, ray.dir);
    float b = - 2.0f * dot(sphere.center - ray.origin, ray.dir);
    float c = dot(sphere.center - ray.origin, sphere.center - ray.origin) - sphere.radius * sphere.radius;

    float det = b*b - 4.0f*a*c;
    
    if(det < 0)
    {
        return false;
    }

    t_hit = (-b - sqrt(det)) / (2.0f * a);
    hit_point = ray.origin + ray.dir * t_hit;
    normal = (hit_point - sphere.center) / sphere.radius;

    return true;
}

vec3 reflect(Ray ray, vec3 normal)
{
    return ray.dir - 2 * dot(normal, ray.dir) * normal;
}

/*
 *  Should take in unit vector
 */
vec3 skyColor(Ray ray)
{
    float t = 0.5 * (ray.dir.y + 1.0f);
    return (1.0 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    // return vec3(210.0f/255, 222.0f/255, 228.0f/255);
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

vec3 randVector(vec3 normal)
{
    return normalize(normal + vec3(rand(normal.xy), rand(normal.yz), rand(normal.xz)));
}

vec3 randVectorInHemisphere(vec3 normal)
{
    while(true)
    {
        vec3 rand_vec = randVector(normal);
        if(dot(rand_vec, normal) > 0)
        {
            return rand_vec;
        }
    }
}
