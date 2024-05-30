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

#define SCREEN_W 768
#define SCREEN_H 768


class Camera
{
    glm::mat4 rotation{1.0f};
    glm::mat4 rotation_while_moving{1.0f};

    glm::mat4 view_matrix{1.0f};

    glm::vec3 position{0.0f, 0.0f, 2.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f}; // Unit vector centered at origin

    float fovy;
    float aspect_ratio;
    float near_clip;
    float far_clip;

    float speed;
    
public:
    // Initialization
    Camera(float _fovy, float _aspect_ratio, float _near_clip, float _far_clip);

    void rotateCamera(float dx, float dy);
    void translateCamera(float dx, float dy);
    
    glm::mat4 getViewMatrix();
};


#endif /* camera_hpp */
