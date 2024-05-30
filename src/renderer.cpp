//
//  renderer.cpp
//  RayTracer
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
    setUniforms();
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
 * Set uniforms for view and rotation matrix from camera object
 */
void Renderer::setUniforms()
{
    GLuint view = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera.view_matrix[0][0]);

    GLuint rotation = glGetUniformLocation(shader_program, "rotation");
    glUniformMatrix4fv(rotation, 1, GL_FALSE, &camera.rotation_matrix[0][0]);
}

/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}
