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
    float dtheta_x = -(dx/768.0f) * M_PI/(180.0f/fovy);
    float dtheta_y = (dy/768.0f) * M_PI/(180.0f/fovy);

    float angle = M_PI/2.0f;
    
    theta_y = theta_y_reference + dtheta_y;

    // Rotate horizontally
    direction = rotate(direction_reference, -dtheta_x, up);
    rotation_matrix = rotate(rotation_reference_matrix, dtheta_x, up);

    vec3 try_direction = rotate(direction, -dtheta_y, right);
    vec3 floor = vec3(0.0f, 1.0f, 0.0f);
    if(abs(theta_y) <= angle - 0.3175604293f)
    {
        direction = rotate(direction, -dtheta_y, right);
        rotation_matrix = rotate(rotation_matrix, dtheta_y, right);
    }
    else
    {
        if(theta_y > 0)
        {
            theta_y = angle - 0.3175604293f;
            direction = rotate(direction, -(angle - 0.3175604293f - theta_y_reference), right);
            rotation_matrix = rotate(rotation_matrix, angle - 0.3175604293f - theta_y_reference, right);
        }
        else
        {
            theta_y = -(angle - 0.3175604293f);
            direction = rotate(direction, angle - 0.3175604293f + theta_y_reference, right);
            rotation_matrix = rotate(rotation_matrix, -(angle - 0.3175604293f + theta_y_reference), right);
        }
    }

    right = rotate(right_reference, -dtheta_x, up);
    foward = rotate(foward_reference, -dtheta_x, up);
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
    theta_y_reference = theta_y;
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
