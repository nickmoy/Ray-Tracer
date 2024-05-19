//
//  renderer.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/6/24.
//

#include "renderer.hpp"
#include <glm/ext/matrix_clip_space.hpp>

using namespace glm;


Renderer::Renderer()
{
    initBuffers();
    initShaders(vert_shader, "Shaders/circle.vert", frag_shader, "Shaders/circle.frag", shader_program);
}

/*
 * Call OpenGL functions to render to screen.
 * Will draw each pixel by specifying its color from light calculations.
 */
void Renderer::render()
{
    // Draw circles
    // for(Circle *circle : physics->objects)
    // {
    //     glBindVertexArray(VAO[1]);
    //     glUniform1f(glGetUniformLocation(shader_program, "RADIUS"), RADIUS);
    //     setCircleCenter(vec3(circle->current_pos, 0.0f));
    //     setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //     glDrawArrays(GL_TRIANGLE_FAN, 0, (GLuint)sizeof(square_vertices)-2);
    // }
}


/*
 * Initialize vertex buffer (VBO) and vertex array object (VAO).
 */
void Renderer::initBuffers()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*SCREEN_W*SCREEN_H, &pixels[0], GL_DYNAMIC_READ);

    // // Generate buffers
    // glGenBuffers(2, VBO);
    // glGenVertexArrays(2, VAO);
    // 
    // // 1st Buffer containing vertices for outer circle stage
    // glBindVertexArray(VAO[0]);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // // glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*(circle_vertices.size()), &circle_vertices[0], GL_STATIC_READ);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 
    // glEnableVertexAttribArray(0);
    // 
    // // 2nd Buffer containing quads for each circle object
    // glBindVertexArray(VAO[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // // glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*4, &square_vertices[0], GL_STATIC_READ);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
}


/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}
