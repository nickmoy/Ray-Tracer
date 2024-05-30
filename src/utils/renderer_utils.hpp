#ifndef renderer_utils_hpp
#define renderer_utils_hpp

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <string>

#define GL_SILENCE_DEPRECATION

// Creating shaders
std::string loadShaderSource(const std::string& filename);
int compileShaderError(GLuint shader);
int linkShadersError(GLuint shader_program);
int initShaders(GLuint &vert_shader, std::string vert_path, GLuint &frag_shader, std::string frag_path, GLuint &shader_program);

// Util functions
void setColor(GLuint shader_program, glm::vec4 color);
glm::vec4 randomColor();
void setCircleCenter(GLuint shader_program, glm::vec3 center);
void setRotationMatrix(GLuint shader_program, glm::mat4 &rotation, glm::mat4 &rotation_while_moving, float dx, float dy);
void print4x4Matrix(float **matrix);


#endif /* renderer_utils.hpp */
