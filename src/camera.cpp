#include "camera.hpp"
#include "glm/matrix.hpp"
#include "utils/renderer_utils.hpp"
#include <glm/ext/matrix_transform.hpp> // This glm library has the rotation view_matrix
#include <glm/ext/matrix_clip_space.hpp> // This glm library has the perspective(frustum) view_matrix
#include <glm/gtx/rotate_vector.hpp> // This glm library has the rotate(vector) function
#include <math.h>
#include <iostream>

using namespace glm;


/*
 * @param fovy is the degrees between top and bottom of field of vision in degrees
 * @param aspect_ratio is the ratio of height/width
 * @param nearClip is the beginning of frustum
 * @param farClilp is the end of frustum
 */
Camera::Camera(float _fovy, float _aspect_ratio, float _near_clip, float _far_clip)
{
    fovy = _fovy;
    aspect_ratio = _aspect_ratio;
    near_clip = _near_clip;
    far_clip = _far_clip;

    translateCamera(0, 0);
}

void Camera::rotateCamera(float dx, float dy)
{
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

    float theta_x = -(dx/768.0f) * 2.0f * M_PI/(180.0f/fovy * 2.0f);
    float theta_y = (dy/768.0f) * 2.0f * M_PI/(180.0f/fovy * 2.0f);

    direction = rotate(direction, theta_x, up);
    direction = rotate(direction, theta_y, up);

    rotation_matrix = glm::rotate(rotation_reference_matrix, theta_x, up);
    rotation_matrix = glm::rotate(rotation_matrix, theta_y, right);
}

void Camera::doneRotating()
{
    rotation_reference_matrix = rotation_matrix;
}

void Camera::translateCamera(float dx, float dy)
{
    vec3 up = vec3(-direction.y, direction.x, direction.z);
    vec3 right = vec3(direction.y, -direction.z, direction.y);

    float t_x = (dx/768.0f) * speed;
    float t_y = (dy/768.0f) * speed;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}
