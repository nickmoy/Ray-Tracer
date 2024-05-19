#ifndef renderer_utils_hpp
#define renderer_utils_hpp

#include <string>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <string>

#define GL_SILENCE_DEPRECATION


std::string loadShaderSource(const std::string& filename);
int compileShaderError(GLuint shader);
int linkShadersError(GLuint shader_program);
int initShaders(GLuint vert_shader, std::string vert_path, GLuint frag_shader, std::string frag_path, GLuint shader_program);
void setColor(GLuint shader_program, glm::vec4 color);
glm::vec4 randomColor();
void setCircleCenter(GLuint shader_program, glm::vec3 center);
glm::mat4 getViewMatrix(glm::vec3 aPos);


#endif /* renderer_utils.hpp */
