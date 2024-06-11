#include "camera.hpp"
#include "glm/common.hpp"
#include "glm/geometric.hpp"


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

    theta_y_reference = 0.0f;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::rotateCamera(float dx, float dy)
{
    float theta_x = -(dx/768.0f) * M_PI/(180.0f/fovy);
    float theta_y = (dy/768.0f) * M_PI/(180.0f/fovy);

    // Rotate horizontally
    direction = rotate(direction_reference, -theta_x, up);
    direction = rotate(direction, -theta_y, right);
    rotation_matrix = rotate(rotation_reference_matrix, theta_x, up);
    rotation_matrix = rotate(rotation_matrix, theta_y, right);

    // vec3 floor = vec3(0.0f, -1.0f, 0.0f);
    // if(glm::abs(theta_y_reference) > (M_PI / 2) - 0.15f)
    // {
    //     direction = rotate(direction, theta_y, right);
    //     rotation_matrix = glm::rotate(rotation_matrix, -theta_y, right);
    // }

    vec3 try_direction = rotate(direction, -theta_y, right);
    vec3 floor = vec3(0.0f, -1.0f, 0.0f);
    if(dot(try_direction, floor) <= 0.95f)
    {
        direction = try_direction;
        rotation_matrix = rotate(rotation_matrix, theta_y, right);
    }
            

    right = rotate(right_reference, -theta_x, up);
    foward = rotate(foward_reference, -theta_x, up);
}

void Camera::doneRotating()
{
    rotation_reference_matrix = rotation_matrix;
    // mat4 reverse = inverse(rotation_matrix);
    // up = reverse[1];
    // right = reverse[0];
    direction_reference = direction;
    right_reference = right;
    foward_reference = foward;
}

void Camera::translateCameraLeft()
{
    position += -right * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::translateCameraRight()
{
    position += right * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::translateCameraUp()
{
    position += up * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::translateCameraDown()
{
    position += -up * SPEED;
    if(position.y < EPS)
    {
        position.y = 0;
    }

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::translateCameraFoward()
{
    position += foward * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}

void Camera::translateCameraBack()
{
    position += -foward * SPEED;

    mat4 temp_vm = mat4{1.0f};
    // For reference, glm::mat4's are an array of column vectors
    view_matrix = glm::translate(temp_vm, -position);
}
