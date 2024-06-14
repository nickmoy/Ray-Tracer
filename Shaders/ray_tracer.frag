#version 330 core

#define M_PI 3.141592653589
#define NUM_OBJS 4
#define NUM_BOUNCES 6
#define NUM_SAMPLES 5
#define T_MIN 0.001f
#define T_MAX 1000.0f

const int LAMBERTIAN = 0;
const int MIRROR = 1;
const int FLOOR = 2;
int seed = 42069;

layout(location = 0) out vec4 FragColor;

layout (std140) uniform matrices
{
    mat4 view;
    mat4 rotation;
    vec3 sky;
    float fovy;
};

uniform float time;

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
    // Sphere(vec3(0.7f, 0.5f, -2.0f), 0.5f, Material(1, vec3(0.85f))),
    // Sphere(vec3(-0.7f, 0.3f, -2.0f), 0.3f, Material(0, vec3(186.0f/255, 121.0f/255, 86.0f/255))),
    // Sphere(vec3(0.0f, -100.0f, -2.0f), 100.0f, Material(3, vec3(80.0f/420, 110.0f/420, 173.0f/420)))

    // Sphere(vec3(-0.6f, 0.3f, -2.0f), 0.3f, Material(LAMBERTIAN, vec3(186.0f/255, 121.0f/255, 86.0f/255))),
    // Sphere(vec3(0.6f, 0.5f, -2.0f), 0.5f, Material(MIRROR, vec3(0.85f))),
    // Sphere(vec3(0.0f, -100.0f, -2.0f), 100.0f, Material(FLOOR, vec3(80.0f/420, 110.0f/420, 173.0f/420)))

    Sphere(vec3(-0.3f, 0.3f, -2.0f), 0.3f, Material(LAMBERTIAN, vec3(1.0f, 0.0f, 0.0f))),
    Sphere(vec3(0.3f, 0.3f, -2.0f), 0.3f, Material(LAMBERTIAN, vec3(0.0f, 0.0f, 1.0f))),
    Sphere(vec3(0.0f, 0.5f, -3.2f), 0.5f, Material(MIRROR, vec3(0.85f))),
    Sphere(vec3(0.0f, -1000.0f, -2.0f), 1000.0f, Material(FLOOR, vec3(80.0f/420, 110.0f/420, 173.0f/420)))
);


float smoothClamp(float x, float a, float b);
float rand(vec2 co, vec2 normal_co);
vec3 reflect(Ray ray, vec3 normal);
void radiance(Ray ray, out Ray ray_next, out vec3 attenuation, inout bool stop, inout vec2 seed, in int depth);
bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal);
vec3 skyColor(Ray ray);
vec3 randUnitVector(Ray ray, vec3 normal);
vec3 randVectorInHemisphere(vec3 normal);
vec3 shootRay(Ray ray);
int hash(inout int seed);
vec3 hashOld33(vec3 p);
vec3 hash32(vec2 p);
vec3 random_in_unit_sphere(vec2 p);
vec2 hash22(vec2 p);


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
        vec2 seed = hash22(ray.dir.xy * 999.0f + float(i) + time);
        radiance(ray, ray_next, attenuation, stop, seed, NUM_BOUNCES - i);
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
void radiance(Ray ray, out Ray ray_next, out vec3 attenuation, inout bool stop, inout vec2 seed, in int depth)
{
    float closest_hit = 2000.0f;
    for(int i = 0; i < NUM_OBJS; i++)
    {
        Sphere sphere = m_objects[i];
        sphere.center = (rotation * view * vec4(sphere.center, 1.0f)).xyz;

        float t_hit = 1.0f;
        vec3 hit_point = vec3(1.0f);
        vec3 normal = vec3(1.0f);

        bool did_hit = intersectSphere(ray, sphere, t_hit, hit_point, normal);

        if(did_hit && t_hit < closest_hit && t_hit > T_MIN && t_hit < T_MAX)
        {
            closest_hit = t_hit;
            ray_next.origin = hit_point;
            // ray_next.dir = normalize(normal + randUnitVector(ray, normal));
            if(sphere.material.type == LAMBERTIAN)
            {
                ray_next.dir = normalize(normal + random_in_unit_sphere(seed * 999.0f + float(depth)));
                attenuation = sphere.material.albedo;
            }
            else if(sphere.material.type == MIRROR)
            {
                ray_next.dir = normal;
                attenuation = sphere.material.albedo;
            }
            else if(sphere.material.type == FLOOR)
            {
                ray_next.dir = normalize(normal + random_in_unit_sphere(seed * 999.0f + float(depth)));
                attenuation = sphere.material.albedo;
            }
        }
    }
    
    // No hit
    if(closest_hit >= 1000.0f)
    {
        stop = true;
        ray_next.origin = vec3(0.0f);
        ray_next.dir = vec3(0.0f, 0.0f, 0.0f);
        attenuation = skyColor(ray);
    }
}

bool intersectSphere(Ray ray, Sphere sphere, out float t_hit, out vec3 hit_point, out vec3 normal)
{
    vec3 oc = sphere.center - ray.origin;

    float a = dot(ray.dir, ray.dir);
    float b = - 2.0f * dot(oc, ray.dir);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    float det = b*b - 4.0f*a*c;
    
    if(det < 0.0f)
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
    float t = 0.5 * (dot(ray.dir, sky) + 1.0f);
    return (1.0 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

float smoothClamp(float x, float a, float b)
{
    float t = clamp(x, a, b);
    return t != x ? t : b + (a - b)/(1. + exp((b - a)*(2.*x - a - b)/((x - a)*(b - x))));
}

float rand(vec2 co, vec2 normal_co)
{
    // return fract(sin(dot(co + normal_co, vec2(12.9898, 78.233))) * 43758.5453f);
    // return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453f);
    float t = 12.9898f*co.x*normal_co.x + 78.233f*co.y*normal_co.y; 
    return fract((43758.5453f+t) * sin(t));
    // seed += 69;
    // return fract((0.5f * sin(fract(69.12345f * seed)*4.0f) + 0.5f) * 43758.5453f);
}

vec3 randUnitVector(Ray ray, vec3 normal)
{
    // return normalize(vec3(hash(seed), hash(seed+1), hash(seed+2)));
    // return normalize(hashOld33(normal));
    return normalize(vec3(rand(ray.dir.xy, normal.xz), rand(ray.dir.xz, normal.yz), rand(ray.dir.yz, normal.xy)));
}

vec3 randVectorInHemisphere(vec3 normal)
{
    return normal;
    // while(true)
    // {
    //     vec3 rand_vec = randUnitVector(normal);
    //     if(dot(rand_vec, normal) > 0)
    //     {
    //         return rand_vec;
    //     }
    // }
}

int hash(inout int seed)
{
    seed += 69;
    seed += ( seed << 10 );
    seed ^= ( seed >>  6 );
    seed += ( seed <<  3 );
    seed ^= ( seed >> 11 );
    seed += ( seed << 15 );
    return seed;
}

vec3 hashOld33(vec3 p)
{
	p = vec3( dot(p,vec3(127.1f,311.7f, 74.7f)),
			  dot(p,vec3(269.5f,183.3f,246.1f)),
			  dot(p,vec3(113.5f,271.9f,124.6f)));

	return fract(sin(p)*43758.5453123f);
}

vec3 hash32(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy+p3.yzz)*p3.zyx);
}

vec3 random_in_unit_sphere(vec2 p) {
    vec3 rand = hash32(p);
    float phi = 2.0 * M_PI * rand.x;
    float cosTheta = 2.0 * rand.y - 1.0;
    float u = rand.z;

    float theta = acos(cosTheta);
    float r = pow(u, 1.0 / 3.0);

    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    return vec3(x, y, z);
}

vec2 hash22(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}
