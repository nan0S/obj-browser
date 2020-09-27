#include "App.hpp"

namespace fs = std::filesystem;

void App::start(const std::string& obj_path,
                const std::string& tex_path)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        getchar();
        return;
    }

    if (!Window::create(1000, 1000, "Obj Browser"))
    {
        fprintf(stderr, "Failed to create window.\n");
        getchar();
        glfwTerminate();
        return;
    }

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW.\n");
        getchar();
        glfwTerminate();
        return;
    }

    init(obj_path, tex_path);
    run();
}

void App::init(const std::string& obj_path,
               const std::string& tex_path)
{
    // initialization
    glfwSetInputMode(Window::get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
    glfwSetCursorPos(Window::get(), Window::getWidth() / 2, Window::getHeight() / 2);
    last_xpos = Window::getWidth() / 2;
    last_ypos = Window::getHeight() / 2;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // vertex buffer object, shaders
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    shader = Shader::load("../shader/vertex.glsl", "../shader/fragment.glsl");
    glUseProgram(shader);

    texsamp_loc = glGetUniformLocation(shader, "textureSampler");
    mvp_loc = glGetUniformLocation(shader, "mvp");
    model_loc = glGetUniformLocation(shader, "model");
    eye_loc = glGetUniformLocation(shader, "eyePosition");
    light_loc = glGetUniformLocation(shader, "lightPos");

    scale_loc = glGetUniformLocation(shader, "scale");
    pers_loc = glGetUniformLocation(shader, "pers");
    lac_loc = glGetUniformLocation(shader, "lac");
    oct_loc = glGetUniformLocation(shader, "octaves");
    light_power_loc = glGetUniformLocation(shader, "light_power");

    tex_mode_loc = glGetUniformLocation(shader, "textureOn");
    noise_mode_loc = glGetUniformLocation(shader, "noiseOn");
    lights_on_loc = glGetUniformLocation(shader, "lightsOn");

    time_loc = glGetUniformLocation(shader, "time");

    GLuint texture;
    std::string ext = fs::path(tex_path).extension();    
    for (int i = 0; i < ext.size(); ++i)
        ext[i] = std::tolower(ext[i]);

    if (ext == ".bmp")
        texture = Texture::loadBMP(tex_path.c_str());
    else if (ext == ".dds")
        texture = Texture::loadDDS(tex_path.c_str());
    else
        std::cout << "Nieznane rozszerzenie tekstury!\n";

    objl::Loader loader;
    bool good = loader.LoadFile(obj_path);

    if (!good)
    {
        std::cout << "Something went wrong when reading object!\n";
        return;
    }

    const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

    vertex_count = vertices.size();

    objl::Vector3 mid;
    float max_dist = 0.f;
    for (auto& v : vertices)
    {
        auto& pos = v.Position;
        mid += pos;
        max_dist = std::max(max_dist, objl::math::MagnitudeV3(pos));
    }

    mid /= vertices.size();
    offset = -glm::vec3(mid.X, mid.Y, mid.Z);
    Camera::init(max_dist);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(objl::Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, TextureCoordinate));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Normal));

    const std::vector<unsigned int>& indices = loader.LoadedIndices;
    index_count = indices.size();

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void App::run()
{
    printHelp();
    while (!Window::shouldClose())
    {   
        input();
        draw();
        printStats();
        glfwPollEvents();
    }
}

void App::input()
{
    static double last = glfwGetTime();
    double now = glfwGetTime();
    float delta = now - last;
    last = now;

    double xpos, ypos;
    glfwGetCursorPos(Window::get(), &xpos, &ypos);

    hor_angle -= mouse_speed * float(xpos - last_xpos); // jestesmy odwroceni plecami defaultowo, dlatego lewa i prawa jest zmieniona
    vert_angle -= mouse_speed * float(ypos - last_ypos); // wspolrzedne ekranu odwrocone (dlatego -)

    last_xpos = xpos;
    last_ypos = ypos;

    if (Window::pressed(GLFW_KEY_LEFT))
        hor_angle += delta;
    if (Window::pressed(GLFW_KEY_RIGHT))
        hor_angle -= delta;
    if (Window::pressed(GLFW_KEY_UP))
        vert_angle += delta;
    if (Window::pressed(GLFW_KEY_DOWN))
        vert_angle -= delta;

    if (Window::pressed(GLFW_KEY_Z))
        Camera::zoom(0.5f);
    if (Window::pressed(GLFW_KEY_X))
        Camera::zoom(-0.5f);

    model = glm::rotate(glm::mat4(1.f), hor_angle, glm::vec3(0.f, 1.f, 0.f)) * 
            glm::rotate(glm::mat4(1.f), vert_angle, glm::vec3(1.f, 0.0, 0.f)) * 
            glm::translate(glm::mat4(1.f), offset);

    static double last_press = 0;
    last_press += delta;

    if (last_press > 0.2)
    {
        if (Window::pressed(GLFW_KEY_Y))
            scale += 1,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_H))
            scale -= 1,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_U))
            pers += 0.1f,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_J))
            pers -= 0.1f,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_I))
            lac += 0.1f,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_K))
            lac -= 0.1f,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_O))
            octaves += 1,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_L))
            octaves -= 1,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_A))
            anim_mode = !anim_mode,
            last_press = 0;
        if (Window::pressed(GLFW_KEY_F))
            lights_on = !lights_on,
            last_press = 0;
	   if (Window::pressed(GLFW_KEY_W))
		  light_power += 0.1f,
		  last_press = 0;
	   if (Window::pressed(GLFW_KEY_S))
		  light_power -= 0.1f,
		  last_press = 0;

        scale = glm::max(scale, 0.f);
        pers = glm::max(pers, 0.f);
        lac = glm::max(lac, 0.f);
        octaves = glm::clamp(octaves, 0, 15);
	   light_power = glm::max(light_power, 0.f);

        if (anim_mode)
            time += 0.01 * delta;

        if (last_press == 0)
        {
            std::cout << "scale: " << scale << "\n";
            std::cout << "persistance: " << pers << "\n";
            std::cout << "lacunarity: " << lac << "\n";
            std::cout << "octaves: " << octaves << "\n\n";
        }
    }

    if (Window::pressed(GLFW_KEY_T))
    {
        tex_mode = true;
        noise_mode = false;
    }
    if (Window::pressed(GLFW_KEY_G))
    {
        tex_mode = false;
        noise_mode = true;
    }
    if (Window::pressed(GLFW_KEY_B))
    {
        tex_mode = true;
        noise_mode = true;
    }
}

void App::draw()
{
    Window::clear();

    glUniform1i(texsamp_loc, 0);

    glm::mat4 mvp = Camera::getMVP(model);
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);

    const glm::vec3& eye_position = Camera::getPosition();
    glUniform3fv(eye_loc, 1, &eye_position[0]);
    glUniform3fv(light_loc, 1, &eye_position[0]);

    glUniform1f(scale_loc, scale);
    glUniform1f(lac_loc, lac);
    glUniform1f(pers_loc, pers);
    glUniform1i(oct_loc, octaves);
    glUniform1f(light_power_loc, light_power);

    glUniform1i(tex_mode_loc, tex_mode);
    glUniform1i(noise_mode_loc, noise_mode);
    glUniform1i(lights_on_loc, lights_on);

    glUniform1f(time_loc, time);

    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);

    Window::display();
}

void App::printHelp()
{
    // std::cout << "\nPrzegladarka plikow \".obj\" z tekstura (.DDS lub .bmp) i szumem Perlina\n\n";
    // std::cout << "STEROWANIE:\n";
    // std::cout << "Obracanie modelu za pomoca myszki lub strzalek\n";
    // std::cout << "Przyblizanie/oddalanie za pomoca kolka myszki lub Z/X\n";
    // std::cout << "T - tryb tekstury\n";
    // std::cout << "G - tryb szumu Perlina\n";
    // std::cout << "B - tryb mieszany\n";
    // std::cout << "F - przelacz oswietlenie\n\n";
    // std::cout << "KONTROLOWANIE SZUMU PERLINA:\n";
    // std::cout << "Szum Perlina jest obliczany za pomoca wzoru:\n";
    // std::cout << "pnoise(t) = sum(persistance^i * noise(t * scale * lacunarity^i)) from i=0 to octaves-1\n";
    // std::cout << "Y - zwiekszenie scale\n";
    // std::cout << "H - zmniejszenie scale\n";
    // std::cout << "U - zwiekszenie persistance\n";
    // std::cout << "J - zmniejszenie persistance\n";
    // std::cout << "I - zwiekszenie lacunarity\n";
    // std::cout << "K - zmniejszenie lacunarity\n";
    // std::cout << "O - zwiekszenie octaves\n";
    // std::cout << "L - zmniejszenie octaves\n";
    // std::cout << "A - przelacz tryb animacji\n\n";

    std::cout << "\nOBJ file browser with texture (format DDS or bmp) and Perlin Noise\n\n";
    std::cout << "Navigation:\n";
    std::cout << "Use your mouse or the arrows to rotate the object.\n";
    std::cout << "Use mouse scroll or the Z/X buttons to zoom in/out.\n";
    std::cout << "T - turn on texture mode (texture is applied to the object)\n";
    std::cout << "G - turn on Perlin Noise mode (Perlin Noise is applied to the object, texture is turned off)\n";
    std::cout << "B - turn on mixed mode (texture and Perlin Noise are applied simultaneously)\n\n";
    
    std::cout << "Light navigation:\n";
    std::cout << "F - toggle lightning\n";
    std::cout << "W - increase light power\n";
    std::cout << "S - decrease light power\n\n";

    std::cout << "Perlin Noise is calculated using the formula below:\n";
    std::cout << "pnoise(t) = sum(persistance^i * noise(t * scale * lacunarity^i)) from i=0 to octaves-1,\n";
    std::cout << "where noise(x) is the original Perlin Noise\n\n";

    std::cout << "Perlin Noise navigation:\n";
    std::cout << "Y - increase scale\n";
    std::cout << "H - decrease scale\n";
    std::cout << "U - increase persistance\n";
    std::cout << "J - decrease persistance\n";
    std::cout << "I - increase lacunarity\n";
    std::cout << "K - decrease lacunarity\n";
    std::cout << "O - increase octaves\n";
    std::cout << "L - decrease octaves\n";
    std::cout << "A - toggle animation mode (if turned on, t increases with time, otherwise not)\n\n";
}

void App::printStats()
{
    static double last = glfwGetTime();
    static double passed = 0;
    static short frames = -1;
    ++frames;

    double now = glfwGetTime();
    passed += now - last;
    last = now;

    if (passed > 1)
    {
        std::cout << "FPS: " << frames << "\n";
        passed = 0;
        frames = 0;
    }
}

App::~App()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteProgram(shader);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
}
