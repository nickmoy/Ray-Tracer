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

#ifndef physics_hpp
#include "physics.hpp"
#endif

#define NUM_VERTICES 256


class Renderer
{
    // Buffer objects
    std::vector<glm::vec3> circle_vertices;
    glm::vec3 *square_vertices;
    GLuint VBO[2];
    GLuint VAO[2];
    
    // Shader objects
    GLuint vert_shader;
    GLuint frag_shader;
    GLuint shader_program;
    
private:
    std::string loadShaderSource(const std::string& filename);
    int compileShaderError(GLuint shader);
    int linkShadersError(GLuint shader_program);
    
public:
    // Physics engine
    Physics *physics;

    //Initialization
    Renderer();
    ~Renderer();
    void initBuffers();
    int initShaders(std::string vert_path, std::string frag_path);
    void addPhysics(Physics *_physics);
    // Rendering math and rendering
    void setCircleCenter(glm::vec3 center);
    void setColor(glm::vec4 color);
    glm::vec4 randomColor();
    std::vector<glm::vec3> buildCircleVertices(float radius);
    glm::vec3* buildSquareVertices();
    void render();
};


#endif /* renderer_hpp */
