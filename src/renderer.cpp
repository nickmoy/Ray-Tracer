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
 * Initialize vertex shader and fragment shader, then compile/link them to
 * shader program.
 */
int Renderer::initShaders(std::string vert_path, std::string frag_path)
{
    // Create Vertex shader
    std::string source = loadShaderSource(vert_path);
    if(source.empty()) { return -1; }
    const char *vert_shader_source = &source[0];
    
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_source, NULL);
    glCompileShader(vert_shader);
    
    // Check for compilation errors
    if(compileShaderError(vert_shader) == -1) { return -1; }
    
    // Create Fragment shader
    source = loadShaderSource(frag_path);
    if(source.empty()) { return -1; }
    const char *frag_shader_source = &source[0];
    
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);
    
    // Check for compilation errors
    if(compileShaderError(frag_shader) == -1) { return -1; }
    
    // Shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    
    // Check for shader program linking error
    if(linkShadersError(shader_program) == -1) { return -1; }
    
    // Use shader program
    glUseProgram(shader_program);
    
    // Initialize uniform RADIUS;
    GLuint RADIUS_location = glGetUniformLocation(shader_program, "RADIUS");
    glUniform1f(RADIUS_location, RADIUS);
    
    // Delete obsolete shader objects once linked and used
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    
    return 0;
}


/*
 * Load shader source code from files in Shaders folder.
 */
std::string Renderer::loadShaderSource(const std::string& filename)
{
    std::ifstream file;
    file.open(filename);
    if (!file) {
        return(NULL);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}


/*
 * Check for shader compilation error.
 */
int Renderer::compileShaderError(GLuint shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    return 0;
}


/*
 * Check for shader linking error.
 */
int Renderer::linkShadersError(GLuint shader_program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "error linking";
        return -1;
    }
    return 0;
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
