//
//  renderer.hpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/6/24.
//

#ifndef renderer_hpp
#define renderer_hpp

#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "physics.hpp"
#include "utils/renderer_utils.hpp"

#define NUM_VERTICES 256


class Renderer
{
    // Buffer objects
    GLuint VBO[2];
    GLuint VAO[2];
    
    // Shader objects
    GLuint vert_shader;
    GLuint frag_shader;
    GLuint shader_program;
    
public:
    // Physics engine
    Physics *physics;

    //Initialization
    Renderer();
    void addPhysics(Physics *_physics);
    void initBuffers();

    // Rendering math and rendering
    void setCircleCenter(glm::vec3 center);
    void setColor(glm::vec4 color);
    glm::vec4 randomColor();
    void render();
};


#endif /* renderer_hpp */
