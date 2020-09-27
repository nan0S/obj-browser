#include "App.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void formatError()
{
    std::cout << "Blad formatu!\nPoprawny format to: ./main.out \"sciezka do .obj\" \"sciezka do tesktury\"\n";
}

int main(int argc, char* argv[])
{
    if (argc < 3)
        return formatError(), 0;
    
    std::string obj_path = std::string(argv[1]);
    std::string tex_path = std::string(argv[2]);

    if (!fs::exists(obj_path))
    {
        std::cout << "Podany plik .obj nie istnieje!\n";
        return 0;
    }
    if (!fs::exists(tex_path))
    {
        std::cout << "Podana tekstura nie istnieje!\n";
        return 0;
    }

    App app;
    app.start(obj_path, tex_path);

    return 0;
}