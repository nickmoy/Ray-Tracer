//
//  camera.hpp
//  RayTracer
//
//  Created by Nicholas Moy on 4/6/24.
//

#ifndef camera_hpp
#define camera_hpp

#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL

#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "utils/renderer_utils.hpp"

#include "glm/matrix.hpp"
#include <glm/ext/matrix_transform.hpp> // This glm library has the rotation view_matrix
#include <glm/ext/matrix_clip_space.hpp> // This glm library has the perspective(frustum) view_matrix
#include <glm/gtx/rotate_vector.hpp> // This glm library has the rotate(vector) function
#include <math.h>

#define SCREEN_W 768
#define SCREEN_H 768
#define SPEED 0.1f


class Camera
{
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f}; // Unit vector centered at origin
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 up_reference{0.0f, 1.0f, 0.0f};
    glm::vec3 right_reference{1.0f, 0.0f, 0.0f};
    
public:
    float fovy;
    float aspect_ratio;
    float near_clip;
    float far_clip;

    glm::mat4 rotation_reference_matrix{1.0f};
    glm::mat4 rotation_matrix{1.0f};

    glm::mat4 view_matrix{1.0f};

    // Initialization
    Camera(float _fovy, float _aspect_ratio, float _near_clip, float _far_clip);

    void rotateCamera(float dx, float dy);
    void translateCameraLeft();
    void doneRotating();
};


#endif /* camera_hpp */
