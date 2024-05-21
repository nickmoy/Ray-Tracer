#include "renderer_utils.hpp"
#include <OpenGL/gltypes.h>
#include <glm/glm.hpp>
#include <glm/glm/gtx/rotate_vector.inl>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace glm;


/*
 * Load shader source code from files in Shaders folder.
 */
std::string loadShaderSource(const std::string& filename)
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
int compileShaderError(GLuint shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    return 0;
}


/*
 * Check for shader linking error.
 */
int linkShadersError(GLuint shader_program)
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
 * Initialize vertex shader and fragment shader, then compile/link them to
 * shader program.
 */
int initShaders(GLuint &vert_shader, std::string vert_path, GLuint &frag_shader, std::string frag_path, GLuint &shader_program)
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
    // GLuint RADIUS_location = glGetUniformLocation(shader_program, "RADIUS");
    // glUniform1f(RADIUS_location, RADIUS);
    
    // Delete obsolete shader objects once linked and used
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    
    return 0;
}

/*
 * Set uniform "color" in vertex shader
 */
void setColor(GLuint shader_program, vec4 color)
{
    glUniform4f(glGetUniformLocation(shader_program, "color"), color.r, color.g, color.b, color.a);
}

/*
 * Return random color
 */
vec4 randomColor()
{
    return vec4((float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100);
}

/*
 * Set uniform "center" in vertex shader to position of circle currently
 * being drawn.
 */
void setCircleCenter(GLuint shader_program, vec3 center)
{
    GLuint center_location = glGetUniformLocation(shader_program, "center");
    glUniform3f(center_location, center.x, center.y, center.z);
}

/*
 * Set uniform "camera_pos" in vert shader
 */
void setCameraPos(GLuint shader_program, vec3 camera_pos)
{
    GLuint camera_pos_loc = glGetUniformLocation(shader_program, "camera_pos");
    glUniform3f(camera_pos_loc, camera_pos.x, camera_pos.y, camera_pos.z);
}

/*
 * Set uniform "view" matrix in frag shader by rotating everything around origin
 * @param dx,dy: The movement of the mouse in pixels
 */
void setViewMatrix(GLuint shader_program, mat4 &view, mat4 &view_while_moving, float dx, float dy)
{
    float theta_x = -(dx/768) * 2.0f*M_PI*(1.0f/4.0f);
    float theta_y = (dy/768) * 2.0f*M_PI*(1.0f/4.0f);

    view_while_moving = glm::rotate(view, theta_x, vec3(0.0f, 1.0f, 0.0f));
    view_while_moving = glm::rotate(view_while_moving, theta_y, vec3(1.0f, 0.0f, 0.0f));

    GLuint view_loc = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_while_moving[0][0]);
}