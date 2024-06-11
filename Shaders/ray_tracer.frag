#version 330 core

#define BRIGHTNESS_COEFF 0.7
#define M_PI 3.141592653589
#define NUM_OBJS 2

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
};

const Sphere m_objects[NUM_OBJS] = Sphere[]
(
    Sphere(vec3(0.5f, -0.5f, -2.0f), 0.5f),
    Sphere(vec3(-0.5f, 0.5f, -2.0f), 0.5f)
);

float smoothClamp(float x, float a, float b);
float rand(vec2 co);
vec3 reflect(vec3 ray, vec3 normal);
vec3 radiance(Ray ray);
bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal);


void main()
{
    // Ray from camera through pixel
    Ray ray;
    ray.origin = vec3(0.0f);
    ray.dir.xyz = (gl_FragCoord.xyz/768.0f) * 2.0f - 1.0f;
    ray.dir.xy *= tan((M_PI/180) * (fovy/2));
    ray.dir.z = -1.0f;

    vec3 color = radiance(ray);

    FragColor = vec4(color, 1.0f);
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

/*
 *  Shoot ray in scene and calculate closest intersection by looping through spheres
 */
vec3 radiance(Ray ray)
{
    // If ray intersects with circle
    // Calculate attenuation based on Lambertian BRDF
    // or use skyColor(ray);
    
    float closest_hit = 2000.0f;
    vec3 color = vec3(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < NUM_OBJS; i++)
    {
        Sphere sphere = m_objects[i];
        sphere.center = (rotation * view * vec4(sphere.center, 1.0f)).xyz;

        float t_hit;
        vec3 hit_point;
        vec3 normal;

        bool did_hit = intersectSphere(ray, sphere, t_hit, hit_point, normal);

        if(did_hit && t_hit < closest_hit)
        {
            closest_hit = t_hit;
            float brightness = max(dot(normal, sky), 0);

            if(t_hit > 0)
            {
                color = vec3(normal * 0.5f + 0.5f) * brightness;
            }
            else
            {
                color = vec3(210.0f/255, 222.0f/255, 228.0f/255);
            }
        }
    }
    
    if(closest_hit >= 1000.0f)
    {
        if(ray.dir.z < 0)
        {
            color = vec3(210.0f/255, 222.0f/255, 228.0f/255);
        }
        else
        {
            color = vec3(80.0f/255, 110.0f/255, 173.0f/255);
        }
    }

    return color;
}

/*
 *  Should take in unit vector
 */
vec3 skyColor(Ray ray)
{
    float t = 0.5 * (ray.dir.y + 1.0f);
    return (1.0 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal)
{
    float a = dot(ray.dir, ray.dir);
    float b = - 2.0f * dot(sphere.center, ray.dir);
    float c = dot(sphere.center, sphere.center) - sphere.radius*sphere.radius;

    float det = b*b - 4.0f*a*c;
    
    if(det < 0)
    {
        return false;
    }

    t_hit = (-b - sqrt(det)) / (2*a);
    hit_point = ray.origin + ray.dir * t_hit;
    normal = normalize(hit_point - sphere.center);

    return true;
}
