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
int initShaders(std::string vert_path, std::string frag_path);


#endif /* renderer_utils.hpp */
