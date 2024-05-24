//
//  renderer.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/6/24.
//

#include "renderer.hpp"
#include "utils/renderer_utils.hpp"
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
    rotation = mat4(1.0f);
    rotation_while_moving = mat4(1.0f);

    initBuffers();
    initShaders(vert_shader, "Shaders/scene.vert", frag_shader, "Shaders/ray_tracer.frag", shader_program);
}

Renderer::~Renderer()
{
    delete square_vertices;
    square_vertices = nullptr;
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
}

/*
 * Rotate the camera during a mouse movement based on the change in pixels from the last mouse position
 * measured in pixels
 */
void Renderer::rotateCamera(float dx, float dy)
{
    setRotationMatrix(shader_program, rotation, rotation_while_moving, dx, dy);
}

/*
 * Update the internal rotation matrix so that rotation contains the correct "rotation" matrix
 * at the end of a mouse click which will then be stored in rotation_while_moving
 */
void Renderer::doneRotatingCamera()
{
    rotation = rotation_while_moving;
}


/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}
