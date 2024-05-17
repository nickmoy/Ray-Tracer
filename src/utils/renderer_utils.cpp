#include "renderer_utils.hpp"


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
int initShaders(GLuint vert_shader, std::string vert_path, GLuint frag_shader, std::string frag_path, GLuint shader_program)
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
