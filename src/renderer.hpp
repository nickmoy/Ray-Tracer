//
//  renderer.hpp
//  RayTracer
//
//  Created by Nicholas Moy on 4/6/24.
//

#ifndef renderer_hpp
#define renderer_hpp

#include "glm/fwd.hpp"
#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL

#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "physics.hpp"
#include "utils/renderer_utils.hpp"
#include "camera.hpp"

#define NUM_VERTICES 256
#define SCREEN_W 768
#define SCREEN_H 768


class Renderer
{
    // Buffer objects
    GLuint VBO = -1;
    GLuint VAO = -1;
    GLuint UBO = -1;

    // glm::vec4 pixels[SCREEN_W][SCREEN_H];
    
    // Shader objects
    GLuint vert_shader = -1;
    GLuint frag_shader = -1;
    GLuint shader_program = -1;

    glm::vec3 square_vertices[4];
    glm::mat4 rotation;
    glm::mat4 rotation_while_moving;
    
public:
    // Physics engine
    Physics *physics;
    Camera camera = Camera(45.0f, 1.0f, 0.1f, 100.0f);

    //Initialization
    Renderer();
    ~Renderer();
    void addPhysics(Physics *_physics);
    void initBuffers();

    // Rendering
    void render();
    void setUniforms();

    // Rendering Math
    void rotateCamera(float dx, float dy);
    void doneRotatingCamera();
};


#endif /* renderer_hpp */
