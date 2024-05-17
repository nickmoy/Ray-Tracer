#ifndef renderer_utils_hpp
#define renderer_utils_hpp

#define GL_SILENCE_DEPRECATION

#include <string>
#include <OpenGL/gl3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


std::string loadShaderSource(const std::string& filename);
int compileShaderError(GLuint shader);
int linkShadersError(GLuint shader_program);
int initShaders(GLuint vert_shader, std::string vert_path, GLuint frag_shader, std::string frag_path, GLuint shader_program);


#endif /* renderer_utils.hpp */
