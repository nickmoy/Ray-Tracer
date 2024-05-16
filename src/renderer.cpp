//
//  renderer.cpp
//  VerletIntegration
//
//  Created by Nicholas Moy on 4/6/24.
//

#include "renderer.hpp"

using namespace glm;


Renderer::Renderer()
{
    circle_vertices = buildCircleVertices(1.0f);
    square_vertices = buildSquareVertices();
    initBuffers();
    initShaders("Shaders/circle.vert", "Shaders/circle.frag");
}

Renderer::~Renderer()
{
    delete square_vertices;
}


/*
 * Initialize vertex buffer (VBO) and vertex array object (VAO).
 */
void Renderer::initBuffers()
{
    // Generate buffers
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);
    
    // 1st Buffer containing vertices for outer circle stage
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*(circle_vertices.size()), &circle_vertices[0], GL_STATIC_READ);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    
    // 2nd Buffer containing quads for each circle object
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*4, &square_vertices[0], GL_STATIC_READ);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

/*
 * Set uniform "center" in vertex shader to position of circle currently
 * being drawn.
 */
void Renderer::setCircleCenter(vec3 center)
{
    GLuint center_location = glGetUniformLocation(shader_program, "center");
    glUniform3f(center_location, center.x, center.y, center.z);
}

/*
 * Set uniform "color" in vertex shader to position of circle currently
 * being drawn.
 */
void Renderer::setColor(vec4 color)
{
    glUniform4f(glGetUniformLocation(shader_program, "color"), color.r, color.g, color.b, color.a);
}


/*
 * Return random color
 */
vec4 Renderer::randomColor()
{
    return vec4((float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100);
}


/*
 * Adds physics object to renderer.
 */
void Renderer::addPhysics(Physics *_physics)
{
    physics = _physics;
}


/*
 * Returns vector of NUM_VERTICES vec3 vertices of (origin-centered) circle
 * arranged for GL_TRIANGLE_FAN.
 */
std::vector<vec3> Renderer::buildCircleVertices(float radius)
{
    std::vector<vec3> vertices;
    float theta = 2*M_PI / NUM_VERTICES;
    for (int i = 0; i < NUM_VERTICES; i++)
    {
        float x = radius * cos(theta * i);
        float y = radius * sin(theta * i);
        float z = 0.0f;
        vertices.push_back(vec3(x, y, z));
    }
    
    vertices.push_back(vec3(radius, 0, 0));
    return vertices;
}

/*
 * Builds vertices for a square with side lengths 2*RADIUS
 * to be drawn with GL_TRIANGLES_STRIP
 */
vec3* Renderer::buildSquareVertices()
{
    vec3 *square_vertices = new vec3[4]
    {
        vec3(-RADIUS, -RADIUS, 0.0f),
        vec3(RADIUS, -RADIUS, 0.0f),
        vec3(RADIUS, RADIUS, 0.0f),
        vec3(-RADIUS, RADIUS, 0.0f)
    };
    
    return square_vertices;
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
