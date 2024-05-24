#include "camera.hpp"
#include <cmath>
#include <glm/ext/matrix_transform.hpp> // This glm library has the rotation matrix
#include <glm/ext/matrix_clip_space.hpp> // This glm library has the perspective(frustum) matrix
#include <glm/gtx/rotate_vector.hpp>
#include <math.h>

using namespace glm;


/*
 * @param fovy is the degrees between top and bottom of field of vision in degrees
 * @param nearClip is the beginning of frustum
 * @param farClilp is the end of frustum
 */
Camera::Camera(float _fovy, float _nearClip, float _farClip)
{
    fovy = _fovy;
    nearClip = _nearClip;
    farClip = _farClip;
}

void Camera::rotateCamera(float dx, float dy)
{
    vec3 up = vec3(-direction.y, direction.x, direction.z);
    float theta_x = ((dx/768.0f) * 2.0f - 1.0f) * M_PI/(180/fovy * 2);
    position = rotate(position, theta_x, up);

    float theta_y = ((dy/768.0f) * 2.0f - 1.0f) * M_PI/(180/fovy * 2);
    position = rotate(position, theta_y, up);
}

void Camera::translateCamera(float dx, float dy)
{
    vec3 right = vec3(direction.y, -direction.z, direction.y);
    vec3 up = vec3(-direction.y, direction.x, direction.z);

    float t_x = ((dx/768.0f) * 2.0f - 1.0f) * speed;
    float t_y = ((dy/768.0f) * 2.0f - 1.0f) * speed;

    direction += t_x * right + t_y * up;
}
