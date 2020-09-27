#include "Shader.hpp"

GLuint Shader::load(const char* vshader_path, 
                   const char* fshader_path)
{
    // create the shaders
    GLuint vshaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // read the vertex shader code from the file
    std::string vshader_code;
    std::ifstream vshader_stream(vshader_path, std::ios::in);
    if (vshader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << vshader_stream.rdbuf();
        vshader_code = sstr.str();
        vshader_stream.close();
    }
    else 
    {
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ!\n", vshader_path);
        getchar();
        return 0;
    }

    // read the fragment shader code from the file
    std::string fshader_code;
    std::ifstream fshader_stream(fshader_path, std::ios::in);
    if (fshader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << fshader_stream.rdbuf();
        fshader_code = sstr.str();
        fshader_stream.close();
    }

    GLint result = GL_FALSE;
    int infolog_length;

    // compile vertex shader
    printf("Compiling shader: %s\n", vshader_path);
    char const* vsource_pointer = vshader_code.c_str();
    glShaderSource(vshaderID, 1, &vsource_pointer, NULL);
    glCompileShader(vshaderID);

    // check vertex shader
    glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vshaderID, GL_INFO_LOG_LENGTH, &infolog_length);
    if (infolog_length > 0)
    {
        char vshader_error_msg[infolog_length + 1];
        glGetShaderInfoLog(vshaderID, infolog_length, NULL, vshader_error_msg);
        printf("%s\n", vshader_error_msg);
        // std::vector<char> vshader_error_msg(infolog_length + 1);
        // glGetShaderInfoLog(vshaderID, infolog_length, NULL, &vshader_error_msg[0]);
        
    }

    // compile fragment shader
    printf("Compiling shader: %s\n", fshader_path);
    char const* fsource_pointer = fshader_code.c_str();
    glShaderSource(fshaderID, 1, &fsource_pointer, NULL);
    glCompileShader(fshaderID);

    // check fragment shader
    glGetShaderiv(fshaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fshaderID, GL_INFO_LOG_LENGTH, &infolog_length);
    if (infolog_length > 0)
    {
        char fshader_error_msg[infolog_length + 1];
        glGetShaderInfoLog(fshaderID, infolog_length, NULL, fshader_error_msg);
        printf("%s\n", fshader_error_msg);
    }

    // link the program
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vshaderID);
    glAttachShader(programID, fshaderID);
    glLinkProgram(programID);

    // check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infolog_length);
    if (infolog_length > 0)
    {
        char program_error_msg[infolog_length + 1];
        glGetProgramInfoLog(programID, infolog_length, NULL, program_error_msg);
        printf("%s\n", program_error_msg);
    }

    // clean up 
    glDetachShader(programID, vshaderID);
    glDetachShader(programID, fshaderID);

    glDeleteShader(vshaderID);
    glDeleteShader(fshaderID);

    return programID;
}
