//
//  renderer.cpp
//  RayTracer
//
//  Created by Nicholas Moy on 4/6/24.
//

#include "renderer.hpp"
#include "SDL2/SDL_timer.h"
#include "utils/renderer_utils.hpp"


using namespace glm;


Renderer::Renderer()
{
    square_vertices[0] = vec3(-1.0f, -1.0f, 0.0f);
    square_vertices[1] = vec3(1.0f, -1.0f, 0.0f);
    square_vertices[2] = vec3(1.0f, 1.0f, 0.0f);
    square_vertices[3] = vec3(-1.0f, 1.0f, 0.0f);

    rotation = mat4(1.0f);
    rotation_while_moving = mat4(1.0f);

    initBuffers();
    initShaders(vert_shader, "Shaders/scene.vert", frag_shader, "Shaders/ray_tracer.frag", shader_program);
}

Renderer::~Renderer()
{
    // delete square_vertices;
    // square_vertices = nullptr;
}

/*
 * Render to screen by drawing a quad (two triangles over the whole screen)
 * and then calculating the color of each pixel in fragment shader using
 * ray tracing
 */
void Renderer::render()
{
    glBindVertexArray(VAO);
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

    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4) + sizeof(vec3) + sizeof(float), NULL, GL_DYNAMIC_DRAW);

    GLuint matrices_index = glGetUniformBlockIndex(shader_program, "matrices");
    // Bind matrices uniform block in shader to index 0
    glUniformBlockBinding(shader_program, matrices_index, 0);

    // Bind UBO to index 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
}

/*
 * Set uniforms for view and rotation matrix from camera object
 */
void Renderer::setUniforms()
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &camera.view_matrix[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &camera.rotation_matrix[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(mat4), sizeof(vec3), &camera.rotation_matrix[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(mat4) + sizeof(vec3), sizeof(float), &camera.fovy);

    GLuint time_loc = glGetUniformLocation(shader_program, "time");
    glUniform1f(time_loc, (float)SDL_GetTicks());

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}
