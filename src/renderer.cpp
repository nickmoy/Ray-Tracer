//
//  renderer.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/6/24.
//

#include "renderer.hpp"
#include "utils/renderer_utils.hpp"
#include <glm/mat4x4.hpp>
#include <iostream>

using namespace glm;


Renderer::Renderer()
{
    square_vertices = new vec3[4]
    {
        vec3(-1.0f, -1.0f, 0.0f),
        vec3(1.0f, -1.0f, 0.0f),
        vec3(1.0f, 1.0f, 0.0f),
        vec3(-1.0f, 1.0f, 0.0f),
    };
    view = mat4(1.0f);
    view_while_moving = mat4(1.0f);

    initBuffers();
    initShaders(vert_shader, "Shaders/scene.vert", frag_shader, "Shaders/ray_tracer.frag", shader_program);
}

/*
 * Render to screen by drawing a quad (two triangles over the whole screen)
 * and then calculating the color of each pixel in fragment shader using
 * ray tracing
 */
void Renderer::render()
{
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLE_FAN, 0, (GLuint)square_vertices->length()-2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


/*
 * Initialize vertex buffer (VBO) and vertex array object (VAO).
 */
void Renderer::initBuffers()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*4, &square_vertices[0], GL_STATIC_READ);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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
 * Rotate the camera during a mouse movement based on the change in pixels from the last mouse position
 * measured in pixels
 */
void Renderer::rotateCamera(float dx, float dy)
{
    setViewMatrix(shader_program, view, view_while_moving, dx, dy);
}

/*
 * Update the internal view and view_while_moving matrixes so that view contains the "view" matrix
 * at the end of a mouse click
 */
void Renderer::doneRotatingCamera()
{
    std::cout << "BEFORE VIEW ADDRESS: " << &view << "\n";
    std::cout << "BEFORE VIEW_WHILE_MOVING ADDRESS: " << &view_while_moving << "\n";
    mat4 temp = view;
    view = view_while_moving;
    view_while_moving = temp;
    std::cout << "AFTER VIEW ADDRESS: " << &view << "\n";
    std::cout << "AFTER VIEW_WHILE_MOVING ADDRESS: " << &view_while_moving << "\n";
    std::cout.flush();
}


/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}
