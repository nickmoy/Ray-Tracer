//
//  renderer.hpp
//  VerletIntegration
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

#define NUM_VERTICES 256
#define SCREEN_W 768
#define SCREEN_H 768


class Renderer
{
    // Buffer objects
    GLuint VBO = -1;
    GLuint VAO = -1;

    // glm::vec4 pixels[SCREEN_W][SCREEN_H];
    
    // Shader objects
    GLuint vert_shader = -1;
    GLuint frag_shader = -1;
    GLuint shader_program = -1;

    glm::vec3 *square_vertices;
    glm::mat4 view;
    
public:
    // Physics engine
    Physics *physics;

    //Initialization
    Renderer();
    void addPhysics(Physics *_physics);
    void initBuffers();

    // Rendering
    void render();
    float* buildSquareVertices();

    // Rendering Math
    void rotateCamera(float dx, float dy);
};


#endif /* renderer_hpp */
