#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <filesystem>

#include "Window.hpp"
#include "Shader.hpp"
#include "ObjLoader.hpp"
// #include "ObjLoader.h"
#include "Texture.hpp"
#include "Camera.hpp"

class App
{
private:
    GLuint vao, shader;
    GLuint vertex_buffer, uv_buffer, normal_buffer;
    GLuint texture;

    GLuint texsamp_loc;
    GLuint mvp_loc;
    GLuint model_loc;
    GLuint eye_loc;
    GLuint light_loc;

    GLuint scale_loc;
    GLuint pers_loc;
    GLuint lac_loc;
    GLuint oct_loc;

    GLuint tex_mode_loc;
    GLuint noise_mode_loc;
    GLuint lights_on_loc;

    GLuint time_loc;

    float scale = 50;
    float pers = 0.5f;
    float lac = 2.f;
    int octaves = 1;
    double time = 0;
    
    bool tex_mode = true;
    bool noise_mode = true;
    bool anim_mode = true;
    bool lights_on = false;

    int vertex_count;
    int index_count;

    float hor_angle = 0.f;
    float vert_angle = 0.f;
    float mouse_speed = 0.005f;

    double last_xpos = 0.0;
    double last_ypos = 0.0;

    glm::vec3 offset;
    glm::mat4 model = glm::mat4(1.f);

    void init(const std::string& obj_path,
              const std::string& tex_path);
    void run();
    void input();
    void draw();
    void printHelp();
    void printStats();

public:
    void start(const std::string& obj_path, 
               const std::string& tex_path);
    ~App();
};