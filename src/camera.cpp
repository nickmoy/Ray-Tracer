#include "camera.hpp"


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

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::rotateCamera(float dx, float dy)
{
    float theta_x = -(dx/768.0f) * M_PI/(180.0f/fovy);
    float theta_y = (dy/768.0f) * M_PI/(180.0f/fovy);

    direction = rotate(direction, theta_x, up);
    direction = rotate(direction, theta_y, right);

    rotation_matrix = glm::rotate(rotation_reference_matrix, theta_x, up);
    rotation_matrix = glm::rotate(rotation_matrix, theta_y, right);

    up = rotate(up_reference, -theta_x, up_reference);
    up = rotate(up, -theta_y, right_reference);
    right = rotate(right_reference, -theta_x, up_reference);
    right = rotate(right, -theta_y, right_reference);

}

void Camera::doneRotating()
{
    rotation_reference_matrix = rotation_matrix;
    // mat4 reverse = inverse(rotation_matrix);
    // up = reverse[1];
    // right = reverse[0];
    up_reference = up;
    right_reference = right;
}

void Camera::translateCameraLeft()
{
    position += right * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}
