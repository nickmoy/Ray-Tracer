#version 330 core

layout(location = 0) out vec4 FragColor;
uniform vec3 camera_pos = vec3(0.0f, 0.0f, 1.0f);
uniform float RADIUS = 0.5f;


bool collideWithSphere(float a, float b, float c);


void main()
{
    vec3 uv = (gl_FragCoord.xyz/768) * 2 - 1;
    // Always set the ray to be shot a z-distance of -1 from the camera
    uv.z = camera_pos.z - 1.0f;

    vec3 ray = uv - camera_pos;

    float a = dot(ray, ray);
    float b = 2.0f * dot(camera_pos, ray);
    float c = dot(camera_pos, camera_pos) - RADIUS*RADIUS;

    if(collideWithSphere(a, b, c))
    {
        FragColor = vec4(uv.x, uv.y, 255.0f/255, 1.0f);
    }
    else
    {
        // vec4 Light_Gray = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0)
        FragColor = vec4(210.0f/255, 222.0f/255, 228.0f/255, 1.0);
    }

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
